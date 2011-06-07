#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H

#pragma once

#ifdef INPUTPROCESSOR_EXPORTS
#define INPUTPROCESSOR_API __declspec(dllexport)
#else
#define INPUTPROCESSOR_API __declspec(dllimport)
#endif

typedef struct {
	bool ctrl;
	bool shift;
	bool alt;
	char key;
} Hotkey;

INPUTPROCESSOR_API void SetMainWindowsHandle(HWND hWnd);
//INPUTPROCESSOR_API void SetMainDir(WCHAR * dir);
INPUTPROCESSOR_API void SetWindowsHooks (HMODULE hMod);
//INPUTPROCESSOR_API HHOOK GetKeyProcHook ();
INPUTPROCESSOR_API HHOOK GetWndProcHook ();
//INPUTPROCESSOR_API HHOOK GetGetMsgProcHook ();

//void sendSingleKey(WORD wVk, DWORD dwFlags);
//void sendKeyStrokes (const std::wstring& s);
//void sendDifference(const std::wstring& contextBefore, const std::wstring& contextAfter);

#ifdef _M_IX86
#pragma data_seg(".KMG32")
#else
#pragma data_seg(".KMG64")
#endif
//HHOOK hKeyHook = NULL;
HHOOK hWndProcHook = NULL;
//HHOOK hGetMsgProcHook = NULL;
//short HotKeys[500] = {0};
//UINT HotKeyCount = 0;
HWND hWndMainWindows = NULL;
//TCHAR szMainDir[MAX_PATH] = {0};
//WCHAR fileNameToLoad[MAX_PATH] = {0};
#pragma data_seg()

//Make sure that section can READ WRITE and SHARE
#ifdef _M_IX86
#pragma comment(linker, "/SECTION:.KMG32,RWS")
#else
#pragma comment(linker, "/SECTION:.KMG64,RWS")
#endif
#endif