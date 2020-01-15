#include "stdafx.h"
#include "dpi.h"
#include <WinUser.h>
#include <Uxtheme.h>
#include <vssym32.h>

#include <CommCtrl.h>
#include <new>

#pragma comment(lib, "UxTheme.lib")

#ifndef USER_DEFAULT_SCREEN_DPI
#define USER_DEFAULT_SCREEN_DPI 96
#endif // !USER_DEFAULT_SCREEN_DPI


// APIs we need, but are not available in all supported OS'
//  - if support for pre-1607 releases of Windows 10 is not required, the code can be a lot simpler

int (WINAPI* ptrGetSystemMetricsForDpi) (int, UINT) = NULL;
BOOL(WINAPI* ptrEnableNonClientDpiScaling) (HWND) = NULL;
UINT(WINAPI* pfnGetDpiForSystem) () = NULL;
UINT(WINAPI* pfnGetDpiForWindow) (HWND) = NULL;
//BOOL(WINAPI* ptrAreDpiAwarenessContextsEqual) (DPI_AWARENESS_CONTEXT, DPI_AWARENESS_CONTEXT) = NULL;
//DPI_AWARENESS_CONTEXT(WINAPI* ptrGetWindowDpiAwarenessContext) (HWND) = NULL;
HRESULT(WINAPI* ptrLoadIconWithScaleDown) (HINSTANCE, PCWSTR, int, int, HICON*) = NULL;

// Convenient loading function, see WinMain
//  - simplified version of https://github.com/tringi/emphasize/blob/master/Windows/Windows_Symbol.hpp

template <typename P>
bool Symbol(HMODULE h, P& pointer, const char* name) {
    if (P p = reinterpret_cast <P> (GetProcAddress(h, name))) {
        pointer = p;
        return true;
    }
    else
        return false;
}
template <typename P>
bool Symbol(HMODULE h, P& pointer, USHORT index) {
    return Symbol(h, pointer, MAKEINTRESOURCEA(index));
}

void LoadSymbols() {
    if (HMODULE hUser32 = GetModuleHandle(L"USER32")) {
        Symbol(hUser32, ptrEnableNonClientDpiScaling, "EnableNonClientDpiScaling");
        Symbol(hUser32, pfnGetDpiForSystem, "GetDpiForSystem");
        Symbol(hUser32, pfnGetDpiForWindow, "GetDpiForWindow");
        Symbol(hUser32, ptrGetSystemMetricsForDpi, "GetSystemMetricsForDpi");
        //Symbol(hUser32, ptrGetWindowDpiAwarenessContext, "GetWindowDpiAwarenessContext");
        //Symbol(hUser32, ptrAreDpiAwarenessContextsEqual, "AreDpiAwarenessContextsEqual");
    }
}

// Generalized DPI retrieval
//  - GetDpiFor(System/Window) available since 1607 / LTSB2016 / Server 2016
//  - GetDeviceCaps is classic way, working way back to XP
//
UINT GetDPI(HWND hWnd) {
    if (hWnd != NULL) {
        if (pfnGetDpiForWindow)
            return pfnGetDpiForWindow(hWnd);
    }
    else {
        if (pfnGetDpiForSystem)
            return pfnGetDpiForSystem();
    }
    if (HDC hDC = GetDC(hWnd)) {
        auto dpi = GetDeviceCaps(hDC, LOGPIXELSX);
        ReleaseDC(hWnd, hDC);
        return dpi;
    }
    else
        return USER_DEFAULT_SCREEN_DPI;
}

LOGFONT CreateDPILogFont(HWND hWnd) {
    auto dpi = GetDPI(hWnd);
    auto dpiSystem = GetDPI(NULL);
    auto hTheme = OpenThemeData(hWnd, L"TEXTSTYLE");

    // theme-dependent stuff gets reloaded here
    //  - note that hTheme can be NULL when XP,Vista,7 is in classic mode
    //    or when compatibility mode is imposed onto the window

    LOGFONT lf;
    if (GetThemeSysFont(hTheme, TMT_MSGBOXFONT, &lf) == S_OK) {
        lf.lfHeight = MulDiv(lf.lfHeight, dpi, dpiSystem);
    }
    else {
        if (GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof lf, &lf)) {
            lf.lfHeight = MulDiv(lf.lfHeight, dpi, dpiSystem);
        }
    }

    return lf;
}