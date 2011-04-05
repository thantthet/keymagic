#include "stdafx.h"
#include "global.h"
#include "InputProcessor.h"

KeyMagicEngine engine;
WCHAR activeFileName[MAX_PATH];
UINT kblIndex = 0;
UINT keyboardLayoutCount = 0;

enum DLLMSG : int {
    KM_SETKEYBOARD = 0x5401,
    KM_LOSTFOCUS = 0x5402,
    KM_GOTFOCUS = 0x5403,
    KM_LISTCHANGED = 0x5404,
    KM_ERROR = 0x5405,
	KM_GETKBLNAME = 0x5406
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

bool ChangeKeyboardLayout(UINT i)
{
	WCHAR file[1000];

	SendMessage(hWndMainWindows, KM_GETKBLNAME, 0, i);
	if (fileNameToLoad[0] == 0) {
		return false;
	}

	wsprintfW(file, L"%s\\KeyboardLayouts\\%s.km2", szMainDir, fileNameToLoad);

	return engine.loadKeyboardFile(file);
}
void SetKeyboardLayoutToDefault()
{
	activeFileName[0] = 0;
	kblIndex = 0;
	SendMessage(hWndMainWindows, KM_GOTFOCUS, kblIndex , (LPARAM)GetFocus());
}

LRESULT CALLBACK HookKeyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode != HC_ACTION){
		return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
	}

	//Key Up
	if (lParam & 0x80000000) {
		return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
	}

	//Menu Mode is active
	if (lParam & 0x10000000) {
		return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
	}

	//Key Down
	else if (lParam) {
		//If no focus
		if (!GetFocus()) {
			DebugPrint(L"No Focus:%d:%X\n", GetCurrentProcessId(), GetForegroundWindow());
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
		}

		DebugPrint(L"HookKeyProc:%d:%X\n", GetCurrentProcessId(), GetForegroundWindow());

		if ((lParam >> 16 & 255) == 255) {
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
		}

		HKL HKLDefault = (HKL)0x04090409;
		HKL HKLCurrent = GetKeyboardLayout(GetCurrentThreadId());

		DebugPrint(L"HKLCurrent=%X\n", HKLCurrent);

		BYTE KeyboardState[256];
		GetKeyboardState(KeyboardState);
		
		for (int i = 0; i < HotKeyCount; i++) {
			Hotkey hotkey;
			hotkey.key = HotKeys[i] & 0xff;
			hotkey.ctrl = HotKeys[i] & 0x100;
			hotkey.alt = HotKeys[i] & 0x200;
			hotkey.shift = HotKeys[i] & 0x400;

			if (hotkey.key != wParam) {
				continue;
			}

			if (hotkey.ctrl != bool(KeyboardState[VK_CONTROL] & 0x80)) {
				continue;
			}
			if (hotkey.alt != bool(KeyboardState[VK_MENU] & 0x80)) {
				continue;
			}
			if (hotkey.shift != bool(KeyboardState[VK_SHIFT] & 0x80)) {
				continue;
			}

			if (i == kblIndex) {
				SetKeyboardLayoutToDefault();
				return true;
			} else if (ChangeKeyboardLayout(i)) {
				wcscpy(activeFileName, fileNameToLoad);
				kblIndex = i;
				SendMessage(hWndMainWindows, KM_GOTFOCUS, kblIndex , (LPARAM)GetForegroundWindow());
				return true;
			}
		}

		DebugPrint(L"activeFileName=%s\n", activeFileName);
		// No Active Keyboard File
		if (activeFileName[0] == 0) {
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
		}

		KeyboardState[VK_CONTROL] = 0;
		KeyboardState[VK_MENU] = 0;
		WCHAR TranslatedChar = 0;

		UINT scanCode = MapVirtualKeyExA(wParam, MAPVK_VK_TO_VSC, HKLCurrent);
		if (scanCode == 0) {
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
		}

		UINT vkey = MapVirtualKeyExA(scanCode, MAPVK_VSC_TO_VK, HKLDefault);
		int ret = ToUnicodeEx(vkey, scanCode, KeyboardState, &TranslatedChar, 1, 0, HKLDefault);

		DebugPrint(L"wParam=%x lParam=%x scanCode=%x vkey=%x char=%c ret=%d\n", wParam, lParam, scanCode, vkey, TranslatedChar, ret);

		int modifiers = 0;
		if (GetKeyState(VK_CONTROL) & 0x80) {
			modifiers |= KeyMagicEngine::CTRL_MASK;
		}
		if (GetKeyState(VK_MENU) & 0x80) {
			modifiers |= KeyMagicEngine::ALT_MASK;
		}
		if (GetKeyState(VK_SHIFT) & 0x80) {
			modifiers |= KeyMagicEngine::SHIFT_MASK;
		}
		if (GetKeyState(VK_CAPITAL) & 0x81) {
			modifiers |= KeyMagicEngine::CAPS_MASK;
		}

		KeyMagicString contextBefore = engine.getContextText()->c_str();

		if (engine.processKeyEvent(TranslatedChar, vkey, modifiers)) {
			KeyMagicString contextAfter = engine.getContextText()->c_str();
			sendDifference(contextBefore, contextAfter);
			return true;
		} else if ( GetKeyState(VK_CONTROL) || GetKeyState(VK_MENU) ){
			engine.reset();
		}
	}

	return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
}

void sendBackspace(int count)
{
	if (!GetFocus() || count < 1)
		return ;

	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.dwExtraInfo = 0;
	ip.ki.time = 0;

	for(int i=0; i < count; i++){
		
		ip.ki.wScan = 255;
		ip.ki.dwFlags = 0;
		ip.ki.wVk = VK_BACK;
		SendInput(1, &ip, sizeof(INPUT));

		ip.ki.wScan = 0;
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		ip.ki.wVk = VK_BACK;
		SendInput(1, &ip, sizeof(INPUT));
	}
}

void sendDifference(const KeyMagicString& contextBefore, const KeyMagicString& contextAfter)
{
	if (contextBefore == contextAfter) {
		return;
	}

	int lengthBefore = contextBefore.length();
	int lengthAfter = contextAfter.length();

	if (contextBefore.length() > contextAfter.length()) {
		sendBackspace(contextBefore.length() - contextAfter.length());
		lengthBefore = lengthAfter;
	}

	KeyMagicString::const_iterator bit, ait;
	int match = 0;
	for (
		bit = contextBefore.begin(), ait = contextAfter.begin();
		bit != contextBefore.end() && ait != contextAfter.end();
		bit++, ait++, match++)
	{
		if (*bit != *ait) {
			break;
		}
	}

	if (match < lengthBefore) {
		sendBackspace(lengthBefore - match);
	}

	if (match < contextAfter.length()) {
		sendKeyStrokes(&(contextAfter.c_str()[match]));
	}
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
	COPYDATASTRUCT* cds;

	switch (cwp->message)
	{
	case WM_COPYDATA:
		cds = (COPYDATASTRUCT*) cwp->lParam;
		switch (cds->dwData)
		{
		case 0xB0B0:
			ZeroMemory(fileNameToLoad, sizeof(fileNameToLoad));
			memcpy(fileNameToLoad, cds->lpData, cds->cbData);
			break;
		case 0xBEEF:
			ZeroMemory(HotKeys, sizeof(HotKeys));
			memcpy(HotKeys, cds->lpData, cds->cbData);
			HotKeyCount = cds->cbData / sizeof(short);
			break;
		}
		break;
	case KM_LISTCHANGED:
		SetKeyboardLayoutToDefault();
		break;
	case KM_SETKEYBOARD:
		WCHAR file[1000];
		wsprintfW(file, L"%s\\KeyboardLayouts\\%s.km2", szMainDir, fileNameToLoad);
		kblIndex = cwp->wParam;

		if (kblIndex == 0) {
			activeFileName[0] = 0;
			break;
		} else if (engine.loadKeyboardFile(file)) {
			engine.reset();
			wcscpy(activeFileName, fileNameToLoad);
		} else {
			ReplyMessage(S_FALSE);
		}
		break;
	// we need to keep track of activity/inactivity of windows as we need to select active one in layout menu
	case WM_ACTIVATE:
		if (LOWORD(cwp->wParam) == WA_INACTIVE) {
			if (isTrayWindow(cwp->hwnd)) break;

			engine.reset();

			wcscpy(fileNameToLoad, activeFileName);
			SendMessage(hWndMainWindows, KM_LOSTFOCUS, kblIndex, (LPARAM)cwp->hwnd);
		}
		else if (LOWORD(cwp->wParam) == WA_ACTIVE || LOWORD(cwp->wParam) == WA_CLICKACTIVE) {
			if (isTrayWindow(cwp->hwnd)) break;

			wcscpy(fileNameToLoad, activeFileName);
			SendMessage(hWndMainWindows, KM_GOTFOCUS, kblIndex, (LPARAM)cwp->hwnd);
		}
		break;
	}

	return CallNextHookEx(hWndProcHook, nCode, wParam, lParam);
}

LRESULT CALLBACK HookGetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return CallNextHookEx(hGetMsgProcHook, nCode, wParam, lParam);

	MSG* msg = (MSG*)lParam;
	switch (msg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		engine.reset();
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		switch (wParam)
		{
			case VK_LEFT:
			case VK_RIGHT:
			case VK_DOWN:
			case VK_UP:
			case VK_END:
			case VK_HOME:
			case VK_PRIOR:
			case VK_NEXT:
				engine.reset();
				break;
		}
		break;
	}

	return CallNextHookEx(hGetMsgProcHook, nCode, wParam, lParam);
}

void sendSingleKey(WORD wVk, DWORD dwFlags)
{
	INPUT ip;

	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.dwFlags = dwFlags;
	ip.ki.time = 0;
	ip.ki.wVk = wVk;

	SendInput(1, &ip, sizeof(INPUT));
}

void sendKeyStrokes (const std::wstring& s)
{
	HWND hwnd = GetFocus();

	int cInputs = s.length() * 2; // need twice for up and down

	INPUT * ip = new INPUT[cInputs];

	for(int i=0, ii=0; i < s.length(); i++, ii++){
		ip[ii].type = INPUT_KEYBOARD;
		ip[ii].ki.dwExtraInfo = 0xDEADC0DE;
		ip[ii].ki.dwFlags = KEYEVENTF_UNICODE;
		ip[ii].ki.time = 0;
		ip[ii].ki.wScan = s.at(i);
		ip[ii].ki.wVk = 0;
		//SendInput(1, &ip, sizeof(INPUT));

		ii++;

		ip[ii].type = INPUT_KEYBOARD;
		ip[ii].ki.dwExtraInfo = 0;
		ip[ii].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
		ip[ii].ki.time = 0;
		ip[ii].ki.wScan = s.at(i);
		ip[ii].ki.wVk = 0;
		//SendInput(1, &ip, sizeof(INPUT));
	}

	int cSent = SendInput(cInputs, ip, sizeof(INPUT));
}

void SetWindowsHooks (HMODULE hMod) {
	hKeyHook = SetWindowsHookEx(WH_KEYBOARD, &HookKeyProc, hMod, 0);
	hGetMsgProcHook = SetWindowsHookEx(WH_GETMESSAGE, &HookGetMsgProc, hMod, NULL);
	hWndProcHook = SetWindowsHookEx(WH_CALLWNDPROC, &HookWndProc, hMod, NULL);
}

void SetMainWindowsHandle(HWND hWnd)
{
	hWndMainWindows = hWnd;
}

void SetMainDir(WCHAR * dir)
{
	wcscpy(szMainDir, dir);
}

HHOOK GetKeyProcHook ()
{
	return hKeyHook;
}

HHOOK GetWndProcHook ()
{
	return hWndProcHook;
}
HHOOK GetGetMsgProcHook ()
{
	return hGetMsgProcHook;
}