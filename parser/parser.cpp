// parser.cpp : Defines the entry point for the console application.
//

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include "stdafx.h"
#include "windows.h"
#include "Shlwapi.h"

#include <iostream>
#include <vector>
#include <wchar.h>
#include <map>

#include "script.h"
#include "lexscanner.h"

using namespace std;

wchar_t source[] = _T("$bb=\"BB\"\r\n")
_T("$b=\"B\"\r\n")
_T("$a = \"A\"\r\n")
_T("$aa = $a+$a\r\n")
_T("$aaa = $aa+$a\r\n")
_T("$aaaa = $a+$a+$a+$a\r\n")
_T("$aa[*] & $b[+] => $2 + u1000 + $1\r\n")
_T("$bb[+] & $a[*] => $1 + u2000 + $2\r\n")
_T("$aa[*] + $b[+] => $1 + $2\r\n")
_T("$aa[+] + $b[*] => $1 + $2\uFFFF");

script s;

#define MAX_NAME 100
#define EOS 0xFFFF

struct ltwcstr
{
  bool operator()(const wchar_t* s1, const wchar_t* s2) const
  {
	return wcscmp(s1, s2) < 0;
  }
};

map<wchar_t*, wchar_t*, ltwcstr> mpVariables;

/* General 
int getLineNum(int pos)
{
	int i ;
	for (i = 0; i < stLinePos.size() && stLinePos[i] < pos; i++){}
	return i;
}

int getPosLine(int pos)
{
	return pos - stLinePos[s.getLineNum(pos)-1];
}*/



/* Parser */

LRESULT checkToken (vector<tyObject> vtObjects, int * ObjIndex, emType Type)
{

	if ( vtObjects.size() - 1 < (*ObjIndex))
	{
		wchar_t str[50];
		swprintf(str, _T("No more token => nObjIndex = %d\n"), *ObjIndex);
		Debug(str);
		return false;
	}

	if ((*ObjIndex) == 7)
	{
		printf("");
	}
	wchar_t str[50];
	swprintf(str, _T("\nObjIndex = %d, TypeWanted = %s\n"), *ObjIndex, Type2Str(Type));
	Debug(str);
	int pos = vtObjects.at(*ObjIndex).iStartIndex;
	wsprintf(str, _T("Parsing at line %d, pos %d\n"), s.getLineNum(pos), s.getPosLine(pos));
	Debug(str);

	if (vtObjects.at(*ObjIndex).Type != Type)
	{
		DumpObject(_T("^^RETURNING FALSE: "), vtObjects.at(*ObjIndex));
		return false;
	}

	LRESULT retValue;
	/*
	if (vtObjects.at(*ObjIndex).Type == T_UNICODE)
	{
		wchar_t aWord[2];
		DumpObject(_T("RETURNING AN UNICODE: "), vtObjects.at(*ObjIndex));
		swscanf(vtObjects.at(*ObjIndex).Value, _T("%x"), aWord);
		retValue =  (LRESULT)aWord;
	}
	else
	{*/
		DumpObject(_T("RETURNING AN OBJECT: "), vtObjects.at(*ObjIndex));
		retValue = (LRESULT)vtObjects.at(*ObjIndex).Value;
	//}

	(*ObjIndex)++;
	return retValue;
}

bool validVarName(wchar_t * VarName)
{
	return true;
}

bool newline(vector<tyObject> vtObjects, int * ObjIndex);
bool condition(vector<tyObject> vtObjects, int * ObjIndex);
bool condition(vector<tyObject> vtObjects, int * ObjIndex);
wchar_t * var(vector<tyObject> vtObjects, int * ObjIndex);

bool varvalue(vector<tyObject> vtObjects, int * ObjIndex)
{
	if (condition(vtObjects, ObjIndex))
		return true;
	return false;
}

bool complexstr(vector<tyObject> vtObjects, int * ObjIndex, wstring * varValue)
{
	
	wchar_t * retVal=0;
	if (retVal = (wchar_t*)checkToken(vtObjects, ObjIndex, T_STRING)) { }
	else if (retVal = (wchar_t*)checkToken(vtObjects, ObjIndex, T_UNICODE)){}
	else if (!(retVal = var(vtObjects, ObjIndex)))
	{
		wchar_t str[50];
		int pos = vtObjects.at(*ObjIndex).iStartIndex;
		swprintf(str, _T("ERROR : Syntax error : %s : Line=%d Pos=%d\n"), vtObjects.at(*ObjIndex).Value, s.getLineNum(pos), s.getPosLine(pos));
		Debug(str);
		return false;
	}

	if (retVal)
		varValue->append(retVal);
	
	int OriginalIndex = *ObjIndex;
	if (checkToken(vtObjects, ObjIndex, T_ANDOP))
		if (!complexstr(vtObjects, ObjIndex, varValue))
			*ObjIndex = OriginalIndex;
	return true;
}

bool vardeclaration(vector<tyObject> vtObjects, int * ObjIndex)
{
	int OriginalIndex = *ObjIndex;

	wchar_t * varname = (wchar_t*)checkToken(vtObjects, ObjIndex, T_IDENTIFIER);
	if (!varname)
	{
		*ObjIndex = OriginalIndex;
		return false;
	}
	if (!checkToken(vtObjects, ObjIndex, T_ASSIGN))
	{
		Debug(_T("Not a vardeclaration\n"));
		*ObjIndex = OriginalIndex;
		return false;
	}
	wstring * varValue = new wstring;
	if (!complexstr(vtObjects, ObjIndex, varValue))
	{
		*ObjIndex = OriginalIndex;
		return false;
	}
	/*wchar_t * varvalue = (wchar_t*)checkToken(vtObjects, ObjIndex, T_STRING);
	if (!varvalue)
	{
		*ObjIndex = OriginalIndex;
		return false;
	}
	if (!varvalue(vtObjects, ObjIndex))
		return false;
	
	if (!validVarName(vtObjects.at(*ObjIndex).Value))
		return false;
	*/
	mpVariables[varname] = (wchar_t*)varValue->c_str();

	newline(vtObjects, ObjIndex);

	return true;
}

bool pattern(vector<tyObject> vtObjects, int * ObjIndex)
{
	if (!checkToken(vtObjects, ObjIndex, T_ANDOP))
		return false;

	condition(vtObjects, ObjIndex);
	return true;
}

wchar_t * var(vector<tyObject> vtObjects, int * ObjIndex)
{
	wchar_t * varname = (wchar_t*)checkToken(vtObjects, ObjIndex, T_IDENTIFIER);
	if (!varname)
		return false;

	wchar_t * varvalue = mpVariables[varname];

	if (!varvalue)
	{
		wchar_t str[50];
		int pos = vtObjects.at((*ObjIndex)-1).iStartIndex;
		wsprintf(str, _T("ERROR: '%s' : undeclared identifier : Line=%d Pos=%d\n"), varname, s.getLineNum(pos), s.getPosLine(pos));
		Debug(str);
		Exit(0);
		return false;
	}

	checkToken(vtObjects, ObjIndex, T_MODIFIER);

	return varvalue;
}

bool context(vector<tyObject> vtObjects, int * ObjIndex)
{
	if (checkToken(vtObjects, ObjIndex, T_STRING))
		return true;
	else if (var(vtObjects, ObjIndex))
		return true;
	//else if (checkToken(vtObjects, ObjIndex, T_UNICODE))
	//	return true;
	else if (checkToken(vtObjects, ObjIndex, T_REFERENCE))
		return true;

	return false;
}

bool condition(vector<tyObject> vtObjects, int * ObjIndex)
{
	if (!context(vtObjects, ObjIndex))
		return false;

	pattern(vtObjects, ObjIndex);

	return true;
}

bool rule(vector<tyObject> vtObjects, int * ObjIndex)
{
	if (!condition(vtObjects, ObjIndex))
		return false;

	if (!checkToken(vtObjects, ObjIndex, T_PRINT))
	{
		Debug(_T("error: Not a rule\n"));
		return false;
	}

	if (!condition(vtObjects, ObjIndex))
		return false;

	newline(vtObjects, ObjIndex);

	return true;
}

bool expression(vector<tyObject> vtObjects, int * ObjIndex);

bool newline(vector<tyObject> vtObjects, int * ObjIndex)
{
	if (!checkToken(vtObjects, ObjIndex, T_NEWLINE))
		return false;
	if (expression(vtObjects, ObjIndex))
		return true;
	return false;
}

bool expression(vector<tyObject> vtObjects, int * ObjIndex)
{
	if (newline(vtObjects, ObjIndex))
		return true;
	else if (vardeclaration(vtObjects, ObjIndex))
		return true;
	else if (rule(vtObjects, ObjIndex))
		return true;
	return false;
}

bool begin_parse(vector<tyObject> vtObjects)
{
	int ObjIndex = 0;
	return expression(vtObjects, &ObjIndex);
}

int _tmain(int argc, _TCHAR* argv[])
{
	s.loadScript(source);
	lexscanner scanner(s);
	vector<tyObject> objects = scanner.getObjects();
	if (objects.size())
		begin_parse(objects);
	system("pause");
	return 0;
}