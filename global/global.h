#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <windows.h>
#include <iostream>
#include <wchar.h>
#include <string.h>
#include <vector>
#include <map>

enum emType {
	T_ADDOP = 1,
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

	pdVK_BACK,
	pdVK_TAB,
	pdVK_RETURN,
	pdVK_SHIFT,
	pdVK_CONTROL,
	pdVK_MENU,
	pdVK_PAUSE,
	pdVK_CAPITAL,
	pdVK_KANJI,
	pdVK_ESCAPE,
	pdVK_SPACE,
	pdVK_PRIOR,
	pdVK_NEXT,
	pdVK_DELETE,

	pdVK_NUMPAD0,
	pdVK_NUMPAD1,
	pdVK_NUMPAD2,
	pdVK_NUMPAD3,
	pdVK_NUMPAD4,
	pdVK_NUMPAD5,
	pdVK_NUMPAD6,
	pdVK_NUMPAD7,
	pdVK_NUMPAD8,
	pdVK_NUMPAD9,

	pdVK_MULTIPLY,
	pdVK_ADD,
	pdVK_SEPARATOR,
	pdVK_SUBTRACT,
	pdVK_DECIMAL,
	pdVK_DIVIDE,

	pdVK_F1,
	pdVK_F2,
	pdVK_F3,
	pdVK_F4,
	pdVK_F5,
	pdVK_F6,
	pdVK_F7,
	pdVK_F8,
	pdVK_F9,
	pdVK_F10,
	pdVK_F11,
	pdVK_F12
};

struct structPREdef
{
	emPreDef preDef;
	const wchar_t * name;
	const wchar_t * value;
};

struct structToken
{
	emType Type;
	const wchar_t *Value;
	int iStartIndex;
	int iLength;
};

extern std::vector<structPREdef> vTpreDef;

extern void push_pre_define_value( emPreDef preDef, const wchar_t * name, const wchar_t * value );
extern void pre_define();
extern wchar_t * Type2Str(emType Type);
extern void Debug(wchar_t * string);
extern void DumpToken(wchar_t * d,structToken kToken);
extern void Exit(int _Code);
extern void PrintLastError();
extern structPREdef * getPreDef(emPreDef preDef);
extern structPREdef * getPreDef(wchar_t * wcSrc, bool isName);


#endif
