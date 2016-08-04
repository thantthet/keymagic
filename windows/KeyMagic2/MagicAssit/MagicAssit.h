#pragma once

enum DLLMSG : int {
	KM_LOSTFOCUS = 0x5401,
	KM_GOTFOCUS = 0x5402,
	KM_SENDKEYSTATES = 0x5403,
	KM_INPUTLANGCHANGE = 0x5404
};

#ifdef MAGICASSIT_EXPORTS
#define MAGICASSIT_API __declspec(dllexport)
#else
#define MAGICASSIT_API __declspec(dllimport)
#endif

MAGICASSIT_API void Dll_SetMainWindowsHandle(HWND hWnd);
MAGICASSIT_API void Dll_SetWindowsHooks(HMODULE hMod);
MAGICASSIT_API void Dll_UnHook();

#ifdef MAGICASSIT_EXPORTS
#ifdef _M_IX86
#pragma data_seg(".KMG32")
#else
#pragma data_seg(".KMG64")
#endif
HHOOK hWndProcHook = NULL;

HWND hWndMainWindows = NULL;
#pragma data_seg()

//Make sure that section can READ WRITE and SHARE
#ifdef _M_IX86
#pragma comment(linker, "/SECTION:.KMG32,RWS")
#else
#pragma comment(linker, "/SECTION:.KMG64,RWS")
#endif
#endif

LRESULT CALLBACK HookWndProc(int nCode, WPARAM wParam, LPARAM lParam);