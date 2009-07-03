#pragma once

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <wchar.h>
#include <iostream>
#include <vector>
#include <Shlwapi.h>
#include <map>
#include <list>

enum emType {
	T_ANDOP,
	T_ASSIGN,
	T_UNICODE,
	T_STRING,
	T_REFERENCE,
	T_NEWLINE,
	T_IDENTIFIER,
	T_MODIFIER,
	T_PRINT,
	T_PREDEFINED
};

enum emPreDef {
	pdNULL = 1,
	pdVK_BACK
};

struct structPREdef
{
	emPreDef preDef;
	wchar_t * name;
	wchar_t * value;
};

static std::vector<structPREdef> vTpreDef;

static void PreDefination()
{
	structPREdef preDef;

	preDef.preDef = pdNULL;
	preDef.name = L"NULL";
	preDef.value = L"";

	preDef.preDef = pdVK_BACK;
	preDef.name = L"VK_BACK";
	preDef.value = L"\x0008";

	vTpreDef.push_back(preDef);
}

static structPREdef * getPreDef(emPreDef preDef)
{
	if (!vTpreDef.size())
		PreDefination();
	for (std::vector<structPREdef>::iterator it = vTpreDef.begin();
		it != vTpreDef.end(); it++)
	{
		if ((*it).preDef == preDef)
			return &(*it);
	}
	return NULL;
}

static structPREdef * getPreDef(wchar_t * wcSrc, bool isName)
{
	if (!vTpreDef.size())
		PreDefination();
	for (std::vector<structPREdef>::iterator it = vTpreDef.begin();
		it != vTpreDef.end(); it++)
	{
		if (!wcsncmp(wcSrc, isName ? (*it).name : (*it).value, wcslen((*it).name)))
		{
			return &(*it);
		}
	}
	return NULL;
}

struct structToken
{
	emType Type;
	wchar_t *Value;
	int iStartIndex;
	int iLength;
};

static wchar_t * Type2Str(emType Type)
{
	switch (Type)
	{
	case T_ANDOP:
		return L"AND_OPERATOR";
		break;
	case T_ASSIGN:
		return L"ASSIGN";
		break;
	case T_UNICODE:
		return L"UNICODE";
		break;
	case T_STRING:
		return L"STRING";
		break;
	case T_REFERENCE:
		return L"REFERENCE";
		break;
	case T_NEWLINE:
		return L"NEWLINE";
		break;
	case T_IDENTIFIER:
		return L"IDENTIFIER";
		break;
	case T_MODIFIER:
		return L"MODIFIER";
		break;
	case T_PRINT:
		return L"PRINT";
		break;
	case T_PREDEFINED:
		return L"PREDEFINED";
		break;
	}
	return L"UNKNOWN";
}

static void Debug(wchar_t * string)
{
	OutputDebugStringW(string);
}

static void DumpToken(wchar_t * d,structToken kToken)
{
	wchar_t str[100];
	swprintf(
		str,
		L"%s => Length = %d, StartIndex = %d, Type = %s, Value = '%s'\n", d, 
		kToken.iLength, kToken.iStartIndex, Type2Str(kToken.Type), kToken.Value
		);
	Debug(str);
}

static void Exit(int _Code)
{
	Debug(L"Application is ending\n");
	system("pause");
	exit(_Code);
}

static void PrintLastError() {
	LPSTR errBuf = NULL;

	FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,
		GetLastError(), 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(LPSTR)&errBuf, 
		0, 
		NULL);

	std::cout << errBuf;

	LocalFree(errBuf);
}


#endif