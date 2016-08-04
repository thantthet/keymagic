// MagicAssit.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MagicAssit.h"

void Dll_SetMainWindowsHandle(HWND hWnd)
{
	hWndMainWindows = hWnd;
}

void Dll_SetWindowsHooks(HMODULE hMod)
{
	hWndProcHook = SetWindowsHookEx(WH_CALLWNDPROC, &HookWndProc, hMod, NULL);
}

void Dll_UnHook()
{
	UnhookWindowsHookEx(hWndProcHook);
}

bool isTrayWindow(HWND hWnd)
{
	TCHAR Name[51];

	Name[50] = 0;

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
	{
		BYTE states[256];
		COPYDATASTRUCT cds;
		cds.dwData = 0x8855;
		cds.cbData = sizeof(states);
		cds.lpData = states;
		GetKeyboardState(states);
		SendMessage(hWndMainWindows, WM_COPYDATA, (WPARAM)cwp->hwnd, (LPARAM)&cds);
	}
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