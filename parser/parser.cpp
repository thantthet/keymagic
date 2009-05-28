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
#include <set>

using namespace std;

wchar_t script[] = _T("SPACE = u0020 + acca\r\n")
//_T("\r\n")
_T("SPACE => SPACE\r\n")
_T("aa[*] & b[+] => $2 + u1000 + $1\r\n")
_T("bb[+] & a[*] => $1 + u2000 + $2\r\n")
_T("aa[*] + b[+] => $1 + $2\r\n")
_T("aa[+] + b[*] => $1 + $2\uFFFF");

#define MAX_NAME 100
#define EOS 0xFFFF

enum emType {
	T_ANDOP,
	T_ASSIGN,
	T_UNICODE,
	T_STRING,
	T_REFERENCE,
	T_NEWLINE,
	T_VARIABLE,
	T_MODIFIER,
	T_PRINT
};

typedef struct
{
	emType Type;
	wchar_t *Value;
	int iStartIndex;
	int iLength;
}
tyObject;

struct ltwcstr
{
  bool operator()(const wchar_t* s1, const wchar_t* s2) const
  {
	return wcscmp(s1, s2) < 0;
  }
};

vector<tyObject> objects;
map<wchar_t*, wchar_t*, ltwcstr> mpVariables;
vector<int> stLinePos;

wchar_t * wPlus = L"+", * wASSIGN = L"=", * wPRINT = L"=>", * wAND = L"&", * wStar = L"*";

void Debug(wchar_t * string)
{
	OutputDebugString(string);
}

void Exit(int _Code)
{
	Debug(_T("Application is ending\n"));
	system("pause");
	exit(_Code);
}

wchar_t * Type2Str(emType Type)
{
	switch (Type)
	{
	case T_ANDOP:
		return _T("AND_OPERATOR");
		break;
	case T_ASSIGN:
		return _T("ASSIGN");
		break;
	case T_UNICODE:
		return _T("UNICODE");
		break;
	case T_STRING:
		return _T("STRING");
		break;
	case T_REFERENCE:
		return _T("REFERENCE");
		break;
	case T_NEWLINE:
		return _T("NEWLINE");
		break;
	case T_VARIABLE:
		return _T("VARIABLE");
		break;
	case T_MODIFIER:
		return _T("MODIFIER");
		break;
	case T_PRINT:
		return _T("PRINT");
		break;
	}
	return _T("UNKNOWN");
}

void DumpObject(wchar_t * d,tyObject kObject)
{
	wchar_t str[100];
	swprintf(str,
		_T("%s => Length = %d, StartIndex = %d, Type = %s, Value = '%s'\n"), d, 
		kObject.iLength, kObject.iStartIndex, Type2Str(kObject.Type), kObject.Value
		);
	Debug(str);
}

int parse_string (wchar_t * s, int scannedIndex)
{
	wchar_t * sEnd;

	scannedIndex++;

	do {
		sEnd = wcschr(&s[scannedIndex], '"');
	} while (sEnd && *(sEnd-1) == '\\');
	

	if (!sEnd)
		return -1;

	int wlen = sEnd - &s[scannedIndex];
	wchar_t * wNew = new wchar_t[wlen+1];
	wcsncpy(wNew, &s[scannedIndex], wlen);
	wNew[wlen] = NULL;

	tyObject kObject;
	kObject.iLength = wlen;
	kObject.iStartIndex = scannedIndex;
	kObject.Type = T_STRING;
	kObject.Value = wNew;

	objects.push_back(kObject);

	DumpObject(_T("New Object Assigned:"), kObject);

	return (wlen+2);
}

wchar_t * WholeWord(wchar_t * s)
{
	//while (*s != EOS)
	//{
		//if (*s == ' ' || *s == '\r' || *s == '\n' || *s == '\t' || *s == '/')
		//	break;
		//s++;
	//}
	return wcspbrk(s, _T("\r\n \t\uFFFF/"));
}

int reference (wchar_t * s,  int scannedIndex)
{
	scannedIndex++;

	//wchar_t * sEnd = wcschr(&s[scannedIndex], ' ');
	wchar_t * sEnd = WholeWord(&s[scannedIndex]);
	if (!sEnd)
		return false;

	for (wchar_t * w = sEnd-1; w >= &s[scannedIndex]; w--)
	{
		if (!isalnum(*w))
		{
			return false;
		}
	}
	
	int wlen = sEnd - &s[scannedIndex];
	wchar_t * wNew = new wchar_t[wlen+1];
	wcsncpy(wNew, &s[scannedIndex], wlen);
	wNew[wlen] = NULL;

	tyObject kObject;

	kObject.iLength = wlen;
	kObject.iStartIndex = scannedIndex;
	kObject.Type = T_REFERENCE;
	kObject.Value = wNew;

	objects.push_back(kObject);

	DumpObject(_T("New Object Assigned:"), kObject);

	return wlen+1;
}

bool ishexa(wchar_t w)
{
	if ((w >= '0' && w <= '9') || (w >= 'a' && w <= 'f') || (w >= 'A' && w <= 'F'))
		return true;
	return false;
}

int unicode (wchar_t * s,  int scannedIndex)
{
	scannedIndex++;

	wchar_t * sEnd = WholeWord(&s[scannedIndex]);
	if (!sEnd)
		return false;

	for (wchar_t * w = sEnd-1; w > &s[scannedIndex]; w--)
	{
		if (!ishexa(*w))
		{
			return false;
		}
	}
	
	int wlen = sEnd - &s[scannedIndex];
	wchar_t * wNew = new wchar_t[wlen+1];
	wcsncpy(wNew, &s[scannedIndex], wlen);
	wNew[wlen] = NULL;

	swscanf(wNew, _T("%x"), wNew);

	tyObject kObject;

	kObject.iLength = wlen;
	kObject.iStartIndex = scannedIndex;
	//kObject.Type = T_UNICODE;
	kObject.Type = T_STRING;
	kObject.Value = wNew;

	objects.push_back(kObject);

	DumpObject(_T("New Object Assigned:"), kObject);

	return wlen+1;
}

bool isModedVar(wchar_t * keyword, int wlen, int * scannedIndex)
{
	if (wlen < 4)
	{
		return false;
	}
	if (keyword[wlen-1] != ']' || keyword[wlen-3] != '[')
	{
		return false;
	}

	int varlen = wlen - 3;

	wchar_t * varname = new wchar_t[varlen+1];
	wcsncpy(varname, keyword,  varlen);
	varname[varlen] = NULL;

	tyObject kObject;

	kObject.iLength = varlen;
	kObject.iStartIndex = *scannedIndex;
	kObject.Type = T_VARIABLE;
	kObject.Value = varname;
	objects.push_back(kObject);

	wchar_t* wModer = new wchar_t[2];
	wModer[1]=NULL;
	wModer[0]=keyword[wlen-2];

	kObject.iLength = 3;
	kObject.iStartIndex = (*scannedIndex) - varlen;
	kObject.Type = T_MODIFIER;
	kObject.Value = wModer;
	objects.push_back(kObject);

	(*scannedIndex) += wlen;

	return true;
}

bool isComment(wchar_t * c, int * index)
{
	int scannedIndex = *index;
	if (c[scannedIndex] == '/' && c[scannedIndex+1] == '/')
	{
		wchar_t * lineEnd = wcspbrk(&c[scannedIndex], _T("\r\n"));
		//lineEnd = wcschr(&c[scannedIndex], '\n');
		(*index) = lineEnd - c;
		return true;
	}
	else if (c[scannedIndex] == '/' && c[scannedIndex+1] == '*')
	{
		scannedIndex++;
		wchar_t * commentEnd = wcsstr(&c[scannedIndex], _T("*/"));
		if (commentEnd)
		{
			(*index) = (commentEnd+2) - c;
			return true;
		}
	}
	Debug(_T("/ cannot be parsed"));
	Exit(0);
	return false;
}

bool scan (wchar_t * s)
{
	int scannedIndex = 0, lineNum=1;

	stLinePos.push_back(scannedIndex);

	while (s[scannedIndex] != EOS)
	{
		tyObject kObject;
		int retlen;

		if (s[scannedIndex] == '\r' || s[scannedIndex+1] == '\n')
		{
			kObject.iLength = 2;
			kObject.iStartIndex = scannedIndex;
			kObject.Type = T_NEWLINE;
			kObject.Value = _T("\r\n");
			objects.push_back(kObject);

			DumpObject(_T("New Object Assigned:"), kObject);
			stLinePos.push_back(scannedIndex+2);

			scannedIndex+=2;
			continue;
		}

		while (s[scannedIndex] == ' ' || s[scannedIndex] == '\t') {scannedIndex++; }

		switch (s[scannedIndex])
		{
		case '$':
			retlen = reference(s, scannedIndex);
			if (retlen > 0)
				scannedIndex += retlen;
			else
				return false;
			break;
		case '+':
		case '&':
			kObject.iLength = 1;
			kObject.iStartIndex = scannedIndex;
			kObject.Type = T_ANDOP;
			kObject.Value = wPlus;
			objects.push_back(kObject);

			DumpObject(_T("New Object Assigned:"), kObject);

			scannedIndex++;
			break;
		case '=':
			if (s[scannedIndex+1] == '>')
			{
				kObject.iLength = 2;
				kObject.iStartIndex = scannedIndex;
				kObject.Type = T_PRINT;
				kObject.Value = wPRINT;
				objects.push_back(kObject);

				scannedIndex++;
			}
			else
			{
				kObject.iLength = 1;
				kObject.iStartIndex = scannedIndex;
				kObject.Type = T_ASSIGN;
				kObject.Value = wASSIGN;
				objects.push_back(kObject);
			}

			scannedIndex++;

			DumpObject(_T("New Object Assigned:"), kObject);
			
			break;
		case 'u':
			retlen = unicode(s, scannedIndex);
			if (retlen > 0)
				scannedIndex += retlen;
			else
				return false;
			break;
		case '"':
			retlen = parse_string (s, scannedIndex);
			if (retlen > 0)
				scannedIndex += retlen;
			else
				return false;
			break;
		case '/':
			if (!isComment(s, &scannedIndex))
				return false;
			break;
		default:
			//wchar_t * sEnd = wcschr(&s[scannedIndex], ' ');
			wchar_t * sEnd = WholeWord(&s[scannedIndex]);
			if (!sEnd)
				return false;
			int wlen = sEnd - &s[scannedIndex];
			wchar_t * wNew = new wchar_t[wlen+1];
			wcsncpy(wNew, &s[scannedIndex], wlen);
			wNew[wlen] = NULL;

			if (isModedVar(wNew, wlen, &scannedIndex))
				break;

			kObject.iLength = wlen;
			kObject.iStartIndex = scannedIndex;
			kObject.Type = T_VARIABLE;
			kObject.Value = wNew;

			objects.push_back(kObject);
			scannedIndex += wlen;

			DumpObject(_T("New Object Assigned:"), kObject);
			break;
		}
	}
	stLinePos.push_back(scannedIndex);

	return true;
}

/* Parser */
int getLineNum(int pos)
{
	int i = 0;
	while (stLinePos[i] <= pos) {i++;}
	return i;
}

int getPosLine(int pos)
{
	return pos - stLinePos[getLineNum(pos)-1];
}

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
	wsprintf(str, _T("Parsing at line %d, pos %d\n"), getLineNum(pos), getPosLine(pos));
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
	else if (!(retVal = (wchar_t*)checkToken(vtObjects, ObjIndex, T_UNICODE)))
	{
		wchar_t str[50];
		int pos = vtObjects.at(*ObjIndex).iStartIndex;
		swprintf(str, _T("ERROR : Syntax error : %s : Line=%d Pos=%d\n"), vtObjects.at(*ObjIndex).Value, getLineNum(pos), getPosLine(pos));
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
	wstring varValue;

	int OriginalIndex = *ObjIndex;

	wchar_t * varname = (wchar_t*)checkToken(vtObjects, ObjIndex, T_VARIABLE);
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
	if (!complexstr(vtObjects, ObjIndex, &varValue))
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
	mpVariables[varname] = (wchar_t*)varValue.c_str();

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

bool var(vector<tyObject> vtObjects, int * ObjIndex)
{
	wchar_t * varname = (wchar_t*)checkToken(vtObjects, ObjIndex, T_VARIABLE);
	if (!varname)
		return false;

	wchar_t * varvalue = mpVariables[varname];

	if (!varvalue)
	{
		wchar_t str[50];
		int pos = vtObjects.at((*ObjIndex)-1).iStartIndex;
		wsprintf(str, _T("ERROR: '%s' : undeclared identifier : Line=%d Pos=%d\n"), varname, getLineNum(pos), getPosLine(pos));
		Debug(str);
		Exit(0);
		return false;
	}

	checkToken(vtObjects, ObjIndex, T_MODIFIER);

	return true;
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
	if (scan(script))
		begin_parse(objects);
	system("pause");
	return 0;
}