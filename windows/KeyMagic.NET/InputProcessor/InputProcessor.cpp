#include "stdafx.h"
#include "global.h"
#include "InputProcessor.h"

#ifdef _M_IX86
#define DllName L"InputProcessor.x32.dll"
#else
#define DllName L"InputProcessor.x64.dll"
#endif

//KeyMagicEngine engine;
WCHAR activeFileName[MAX_PATH];
UINT kblIndex = 0;
UINT keyboardLayoutCount = 0;

enum DLLMSG : int {
    //KM_SETKEYBOARD = 0x5401,
	KM_ERROR = 0x5401,
    KM_LOSTFOCUS = 0x5402,
    KM_GOTFOCUS = 0x5403,
    //KM_LISTCHANGED = 0x5404,
	//KM_GETKBLNAME = 0x5406
	KM_INPUTLANGCHANGE = 0x5404,
	KM_SENDKEYSTATES = 0x5405
};

void DebugPrint(LPCWSTR fmt, ...)
{
#ifdef _DEBUG
    WCHAR pBuffer[5000];

    va_list args = NULL;
    va_start(args, fmt);

    wvsprintfW(pBuffer, fmt, args);
	wprintf(pBuffer);

    va_end(args);

	OutputDebugStringW(pBuffer);
#endif
}

bool isTrayWindow(HWND hWnd)
{
	TCHAR Name[51];

	GetClassName(hWnd, Name, 50);
	
	if (lstrcmp(Name, TEXT("Shell_TrayWnd")) == 0)
		return true;
	if (lstrcmp(Name, TEXT("TrayNotifyWnd")) == 0)
		return true;
	if (lstrcmp(Name, TEXT("NotifyIconOverflowWindow")) == 0)
		return true;

	return false;
}

LRESULT CALLBACK HookWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	bool bSuccess = false;

	if (nCode < 0)
		return CallNextHookEx(hWndProcHook, nCode, wParam, lParam);

	CWPSTRUCT* cwp = (CWPSTRUCT*)lParam;

	switch (cwp->message)
	{
	case KM_SENDKEYSTATES:
		BYTE states[256];
		COPYDATASTRUCT cds;
		cds.dwData = 0x8855;
		cds.cbData = sizeof(states);
		cds.lpData = states;
		GetKeyboardState(states);
		SendMessage(hWndMainWindows, WM_COPYDATA, (WPARAM)cwp->hwnd, (LPARAM)&cds);
		break;
	case WM_INPUTLANGCHANGE:
		SendMessage(hWndMainWindows, KM_INPUTLANGCHANGE, cwp->wParam, cwp->lParam);
		break;
	// we need to keep track of activity/inactivity of windows as we need to select active one in layout menu
	case WM_ACTIVATE:
		if (LOWORD(cwp->wParam) == WA_INACTIVE) {
			if (isTrayWindow(cwp->hwnd)) break;

			SendMessage(hWndMainWindows, KM_LOSTFOCUS, 0, (LPARAM)cwp->hwnd);
		}
		else if (LOWORD(cwp->wParam) == WA_ACTIVE || LOWORD(cwp->wParam) == WA_CLICKACTIVE) {
			if (isTrayWindow(cwp->hwnd)) break;

			SendMessage(hWndMainWindows, KM_GOTFOCUS, 0, (LPARAM)cwp->hwnd);
		}
		break;
	}

	return CallNextHookEx(hWndProcHook, nCode, wParam, lParam);
}

//LRESULT CALLBACK HookGetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
//{
//	if (nCode < 0)
//		return CallNextHookEx(hGetMsgProcHook, nCode, wParam, lParam);
//
//	MSG* msg = (MSG*)lParam;
//	switch (msg->message)
//	{
//	case WM_INPUTLANGCHANGE:
//		DebugPrint(TEXT("HookGetMsgProc::WM_INPUTLANGCHANGE"));
//		break;
//	}
//
//	return CallNextHookEx(hGetMsgProcHook, nCode, wParam, lParam);
//}

void SetWindowsHooks (HMODULE hMod) {
	//hKeyHook = SetWindowsHookEx(WH_KEYBOARD, &HookKeyProc, hMod, 0);
	//hGetMsgProcHook = SetWindowsHookEx(WH_GETMESSAGE, &HookGetMsgProc, hMod, NULL);
	hWndProcHook = SetWindowsHookEx(WH_CALLWNDPROC, &HookWndProc, hMod, NULL);
}

void SetMainWindowsHandle(HWND hWnd)
{
	hWndMainWindows = hWnd;
}

//void SetMainDir(WCHAR * dir)
//{
//	wcscpy(szMainDir, dir);
//}

//HHOOK GetKeyProcHook ()
//{
//	return hKeyHook;
//}

HHOOK GetWndProcHook ()
{
	return hWndProcHook;
}

//HHOOK GetGetMsgProcHook ()
//{
//	return hGetMsgProcHook;
//}