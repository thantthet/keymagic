#pragma once

#include "stdafx.h"

void LoadSymbols();

// Generalized DPI retrieval
//  - GetDpiFor(System/Window) available since 1607 / LTSB2016 / Server 2016
//  - GetDeviceCaps is classic way, working way back to XP
//
UINT GetDPI(HWND hWnd);

LOGFONT CreateDPILogFont(HWND hWnd);