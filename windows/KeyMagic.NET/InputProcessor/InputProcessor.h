#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H

#pragma once

//typedef struct {
//	UINT index;
//	int hotkey;
//} HotkeyInt;

typedef struct {
	bool ctrl;
	bool shift;
	bool alt;
	char key;
} Hotkey;

//typedef int (* GetHotkeysProc) (UINT count, HotkeyInt * hotkey);
//typedef int (* GetKeyboardLayoutNameByIndexProc) (UINT index, WCHAR * receiver, UINT size);
//
//typedef struct {
//    HWND HWnd;
//    GetHotkeysProc GetHotkeys;
//    GetKeyboardLayoutNameByIndexProc GetKeyboardLayoutNameByIndex;
//} CommunicationData;

void SetMainWindowsHandle(HWND hWnd);
void SetMainDir(WCHAR * dir);
void SetWindowsHooks ();
HHOOK GetKeyProcHook ();
HHOOK GetWndProcHook ();
HHOOK GetGetMsgProcHook ();

UINT GetHotkeyCount();

void sendSingleKey(WORD wVk, DWORD dwFlags);
void sendKeyStrokes (const std::wstring& s);
void sendDifference(const std::wstring& contextBefore, const std::wstring& contextAfter);

#pragma data_seg(".KMG")
HHOOK hKeyHook = NULL;
HHOOK hWndProcHook = NULL;
HHOOK hGetMsgProcHook = NULL;
short Hotkeys[500] = {0};
HWND hWndMainWindows = NULL;
TCHAR szMainDir[MAX_PATH] = {0};
WCHAR fileNameToLoad[MAX_PATH] = {0};
#pragma data_seg()

//Make sure that section can READ WRITE and SHARE
#pragma comment(linker, "/SECTION:.KMG,RWS")

#endif