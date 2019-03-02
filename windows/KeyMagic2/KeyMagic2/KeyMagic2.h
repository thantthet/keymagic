#pragma once

#include "resource.h"

#define DebugLog(msg) { std::wstringstream ss; ss << msg; OutputDebugStringW(ss.str().c_str()); }

void ReloadKeyboards(HWND hWnd);
void SetShellNotifyIcon(HWND hWnd);