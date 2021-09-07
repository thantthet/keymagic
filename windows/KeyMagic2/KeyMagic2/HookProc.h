#pragma once

#include <Windows.h>
#include <keymagic.h>
#include "User32.h"

class HookProc {
private:
	User32 user32 = User32::shared();

	BYTE KeyboardStates[256];

	HHOOK HH_KEYBOARD_LL;
	HHOOK HH_MOUSE_LL;

	HWINEVENTHOOK HWH_SYSTEM_FOREGROUND;

	HWND ignoreHandleForegroundEvent;

	LRESULT LowLevelKeyboardProc(
		_In_ int    nCode,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);

	void WinEventHookProc(
		HWINEVENTHOOK hWinEventHook,
		DWORD         event,
		HWND          hwnd,
		LONG          idObject,
		LONG          idChild,
		DWORD         idEventThread,
		DWORD         dwmsEventTime);

	LRESULT LowLevelMouseProc(
		int    nCode,
		WPARAM wParam,
		LPARAM lParam
	);

	DWORD handleNumpadKeys(DWORD);
	void SendString(const libkm::KeyMagicString&);
	void SendBackspace(ULONG);

public:
	BOOL InitHooks(HWND mainHwnd);

	static HookProc& shared() {
		static HookProc shared = HookProc();
		return shared;
	}
};