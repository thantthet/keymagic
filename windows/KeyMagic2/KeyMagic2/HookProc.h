#pragma once

#include <Windows.h>

LRESULT CALLBACK LowLevelKeyboardProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	);

BOOL InitHooks(HWND mainHwnd);

extern BYTE KeyboardStates[256];