#include "parser.h"

using namespace std;

//#define DEBUG 1

LRESULT parser::checkToken (int * objIndex, emType Type)
{
	if ( tokens.size() - 1 < (*objIndex))
	{
		wchar_t str[50];
		swprintf(str, _T("No more token => nObjIndex = %d\n"), *objIndex);
		Debug(str);
		return false;
	}

	wchar_t str[50];
	swprintf(str, _T("\nObjIndex = %d, TypeWanted = %s\n"), *objIndex, Type2Str(Type));
	Debug(str);
	int pos = tokens.at(*objIndex).iStartIndex;
	wsprintf(str, _T("Parsing at line %d, pos %d\n"), Script.getLineNum(pos), Script.getPosLine(pos));
	Debug(str);

	if (tokens.at(*objIndex).Type != Type)
	{
		DumpToken(_T("NOT MATCHED: "), tokens.at(*objIndex));
		return false;
	}

	LRESULT retValue;

	DumpToken(_T("MATCHED FOUND: "), tokens.at(*objIndex));
	retValue = (LRESULT)tokens.at(*objIndex).Value;

	(*objIndex)++;
	return retValue;
}

bool parser::validVarName(wchar_t * VarName)
{
	return true;
}

bool parser::complexstr(int * objIndex, wstring * varValue)
{
	
	wchar_t * retVal=0;

	if (retVal = (wchar_t*)checkToken(objIndex, T_STRING)) { }

	else if (retVal = (wchar_t*)checkToken(objIndex, T_UNICODE)){}

	else if (!(retVal = identifier(objIndex)))
	{
		wchar_t str[50];
		int pos = tokens.at(*objIndex).iStartIndex;
		swprintf(str, _T("ERROR : Syntax error : %s : Line=%d Pos=%d\n"), tokens.at(*objIndex).Value, Script.getLineNum(pos), Script.getPosLine(pos));
		Debug(str);
		return false;
	}

	if (retVal)
		varValue->append(retVal);
	
	int OriginalIndex = *objIndex;

	if (checkToken(objIndex, T_ANDOP))
		if (!complexstr(objIndex, varValue))
			*objIndex = OriginalIndex;
	return true;
}

bool parser::vardeclaration(int * objIndex)
{
	int OriginalIndex = *objIndex;

	wchar_t * varName = (wchar_t*)checkToken(objIndex, T_IDENTIFIER);

	if (!varName)
	{
		*objIndex = OriginalIndex;
		return false;
	}

	if (!checkToken( objIndex, T_ASSIGN ))
	{
		Debug(_T("Not a vardeclaration\n"));
		*objIndex = OriginalIndex;
		return false;
	}


	wstring * varValue = new wstring;

	if (!complexstr(objIndex, varValue))
	{
		varValue->~basic_string();
		*objIndex = OriginalIndex;
		return false;
	}

	setVar(varName, (wchar_t*)varValue->c_str());

	delete varValue;

	newline(objIndex);

	return true;
}

void parser::setVar(wchar_t * varName, wchar_t * varValue)
{
	int iLength = wcslen(varValue);
	wchar_t * wsValue = new wchar_t [iLength+1];
	wcscpy(wsValue, varValue);

	kmklf.add_str(wsValue);
	mpVariables[varName] = wsValue;
}

bool parser::pattern(int * objIndex, wstring * outStr)
{
	if (!checkToken(objIndex, T_ANDOP))
		return false;

	condition(objIndex, outStr);
	return true;
}

// Return :	int: VariableIndex
//			int: Modifeir

wchar_t * parser::identifier(int * objIndex)
{
	wchar_t * varName = (wchar_t*)checkToken(objIndex, T_IDENTIFIER);
	if (!varName)
		return false;

	wchar_t * varvalue = mpVariables[varName];

	if (!varvalue)
	{
		wchar_t str[50];
		int pos = tokens.at((*objIndex)-1).iStartIndex;
		wsprintf(str, _T("ERROR: '%s' : undeclared identifier : Line=%d Pos=%d\n"), varName, Script.getLineNum(pos), Script.getPosLine(pos));
		Debug(str);
		Exit(0);
		return false;
	}

	wstring * strValue = new wstring();

	strValue->append<int>(1,getVar(varvalue));

	if (wchar_t * moder = (wchar_t *)checkToken(objIndex, T_MODIFIER))
	{
		strValue->append<int>(1,opMODIFIER);
		strValue->append<int>(1,opANYOF);
	}

	return (wchar_t*)strValue->c_str();
}

int parser::getVar(wchar_t * value)
{
#ifdef DEBUG
	wchar_t wc_integer[2];
	wsprintf(wc_integer, L"%d", kmklf.is_exist(value));
	return (int) wc_integer[0];
#endif
	return kmklf.is_exist(value)+1;
}

bool parser::context(int * objIndex, wstring * outStr)
{
	if (wchar_t * s = (wchar_t*)checkToken(objIndex, T_STRING))
	{
		outStr->append<int>(1,opSTRING);
		outStr->append<int>(1,wcslen(s));
		outStr->append(s);
		return true;
	}

	else if (wchar_t * s = identifier(objIndex))
	{
		outStr->append<int>(1,opVARIABLE);
		outStr->append(s);
		return true;
	}

	else if (wchar_t * s = (wchar_t*)checkToken(objIndex, T_REFERENCE))
	{
		outStr->append<int>(1,opREFERENCE);
		outStr->append(s);
		return true;
	}

	else if (wchar_t * s = (wchar_t*)checkToken(objIndex, T_PREDEFINED))
	{
		outStr->append<int>(1, opPREDEFINED);
		structPREdef * preDef = getPreDef(s, true);
		outStr->append<int>(1, preDef->preDef);
		return true;
	}

	return false;
}

bool parser::condition(int * objIndex, wstring * outStr)
{
	if (!context(objIndex, outStr))
		return false;

	pattern(objIndex, outStr);

	return true;
}

bool parser::rule(int * objIndex)
{
	wstring outStr;
	wstring inStr;

	if (!condition(objIndex, &inStr))
		return false;

	if (!checkToken(objIndex, T_PRINT))
	{
		Debug(_T("error: Not a rule\n"));
		return false;
	}

	if (!condition(objIndex, &outStr))
		return false;

	kmklf.add_rule(inStr.c_str(),outStr.c_str());

	newline(objIndex);

	return true;
}

bool parser::newline(int * objIndex)
{
	if (!checkToken(objIndex, T_NEWLINE))
		return false;

	if (expression(objIndex))
		return true;

	return false;
}

bool parser::expression(int * objIndex)
{	
	if (newline(objIndex))
		return true;

	else if (vardeclaration(objIndex))
		return true;

	else if (rule(objIndex))
		return true;

	return false;
}

bool parser::begin_parse()
{
	int objIndex = 0;
	return expression(&objIndex);
}