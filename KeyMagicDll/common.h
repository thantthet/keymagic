#pragma once

#ifndef _COMMON_H_
#define _COMMON_H_

#include <windows.h>
#include <vector>
#include <Userenv.h>

#include "../parser/klf.h"
#include "InternalEditor.h"

extern HHOOK hKeyHook;
extern HHOOK hWndProcHook;
extern HHOOK hGetMsgHook;
extern HWND hwndKWindows;
extern char szDir[1000];

struct KM_ShortCut{
	BYTE ukey;
	BYTE modkey;
};

static std::vector<KM_ShortCut> vtSC;
static classInternalEditor InternalEditor;

extern void GetShortCuts();
extern void Logger(char* fmt, ...);
extern LPCSTR GetKeyBoard(UINT Index, char * szKBPath);
extern bool LoadKeymapFile(int index);
extern bool TranslateToAscii (UINT *uVKey);
extern Kmklf klf;

#endif