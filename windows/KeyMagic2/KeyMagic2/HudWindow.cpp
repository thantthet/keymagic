#include "stdafx.h"
#include "HudWindow.h"
#include "Resource.h"
#include <time.h>
#include <Shlwapi.h>

BOOL HudWindow::registered = FALSE;

HudWindow::HudWindow(HINSTANCE hInstance, KeyboardManager * manager)
{
	this->hInst = hInstance;
	this->kbdManager = manager;
	this->kbdManager->addOnKeyboardDidChangeHandler([&]() {
		this->onKeyboardDidChange();
	});
	if (!HudWindow::registered) {
		HudWindow::RegisterWindowClass(this->hInst);
		HudWindow::registered = TRUE;
	}
}

HudWindow::~HudWindow()
{
}

void HudWindow::onKeyboardDidChange()
{
	this->Show();
	SetTimer(this->hWnd,
		1,
		1000,
		NULL);
}

//
//  FUNCTION: RegisterWindowClass()
//
//  PURPOSE: Registers the window class.
//
ATOM HudWindow::RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;

	wcex.lpfnWndProc = [](HWND h, UINT m, WPARAM w, LPARAM l) -> LRESULT {
		HudWindow *pThis = (HudWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		if (pThis)
		{
			return pThis->WndProc(h, m, w, l);
		}
		else if (m == WM_CREATE)
		{
			pThis = (HudWindow *)(((CREATESTRUCT *)l)->lpCreateParams);
			SetWindowLongPtr(h, GWLP_USERDATA, (LONG_PTR)pThis);
			return pThis->WndProc(h, m, w, l);
		}
		return DefWindowProc(h, m, w, l);
	};
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = HudWindow::szWindowClass;
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: HudInitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL HudWindow::InitInstance()
{
	HWND hWnd = CreateWindowEx(
		WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
		szWindowClass,
		_T(""),
		WS_POPUP | WS_VISIBLE | WS_SYSMENU,
		0, 0, 0, 0,
		nullptr, nullptr,
		this->hInst,
		this);

	if (!hWnd)
	{
		return FALSE;
	}

	this->hWnd = hWnd;

	return TRUE;
}

//
//  FUNCTION: HudWndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK HudWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			this->Hide();
			break;
		}
		break;
	case WM_NCHITTEST:
		return HTNOWHERE;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void HudWindow::Show()
{
	HBITMAP bitmap = this->CreateBitmap();

	this->SetBitmap(bitmap);
	DeleteObject(bitmap);
}

void HudWindow::Hide()
{
	MoveWindow(this->hWnd, 0, 0, 0, 0, false);
}

void HudWindow::SetBitmapAlpha(HDC hdc, HBITMAP hBmp)
{
	BITMAP bm = { 0 };
	GetObject(hBmp, sizeof(bm), &bm);
	BITMAPINFO* bmi = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD)));
	ZeroMemory(bmi, sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD)));
	bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BOOL bRes = GetDIBits(hdc, hBmp, 0, bm.bmHeight, NULL, bmi, DIB_RGB_COLORS);
	if (!bRes || bmi->bmiHeader.biBitCount != 32) return;
	LPBYTE pBitData = (LPBYTE)LocalAlloc(LPTR, bm.bmWidth * bm.bmHeight * sizeof(DWORD));
	if (pBitData == NULL) return;
	LPBYTE pData = pBitData;
	GetDIBits(hdc, hBmp, 0, bm.bmHeight, pData, bmi, DIB_RGB_COLORS);
	for (int y = 0; y < bm.bmHeight; y++) {
		for (int x = 0; x < bm.bmWidth; x++) {
			pData += 4;
		}
	}
	SetDIBits(hdc, hBmp, 0, bm.bmHeight, pBitData, bmi, DIB_RGB_COLORS);
	LocalFree(pBitData);
	free(bmi);
}

HBITMAP HudWindow::CreateBitmap()
{
	HDC hdc = GetDC(HWND_DESKTOP);

	// Get current keyboard name
	Keyboard *kbd = this->kbdManager->SelectedKeyboard();
	std::wstring name(_T("keymagic is off"));
	if (kbd) {
		name = kbd->name;
	}

	// Create and draw onto memory bitmap
	HDC memDC = CreateCompatibleDC(NULL);
	SetBkMode(memDC, TRANSPARENT);

	// Create Font
	LOGFONT font = { 0 };
	StrCpy(font.lfFaceName, _T("Pyidaungsu"));
	font.lfHeight = -MulDiv(25, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	HGDIOBJ hfnt = CreateFontIndirect(&font);
	
	// Set Font
	SelectObject(memDC, hfnt);

	// Measure Text
	SIZE size;
	GetTextExtentPoint32(memDC, name.c_str(), name.length(), &size);

	LONG bmpWidth = size.cx + (20 * 2);
	LONG bmpHeight = size.cy + (20 * 2);
	HBITMAP bmp = CreateCompatibleBitmap(hdc, bmpWidth, bmpHeight);

	// Set Pen
	SelectObject(memDC, bmp);
	HPEN transparentPen = CreatePen(PS_SOLID, 0, this->transparent);
	SelectObject(memDC, transparentPen);
	// Set Brush
	HBRUSH transparentBrush = CreateSolidBrush(this->transparent);
	SelectObject(memDC, transparentBrush);
	// Draw Rect
	Rectangle(memDC, 0, 0, bmpWidth, bmpHeight);
	// Set Brush
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(memDC, brush);
	RoundRect(memDC, 0, 0, bmpWidth, bmpHeight, 22, 22);

	RECT rect = { 0 };
	rect.right = bmpWidth;
	rect.bottom = bmpHeight;
	SetTextColor(memDC, RGB(0xff, 0xff, 0xff));

	DrawTextEx(memDC, (LPTSTR)name.c_str(), name.length(), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);
	
	//this->SetBitmapAlpha(memDC, bmp);
	DeleteObject(hfnt);
	DeleteObject(brush);
	DeleteDC(memDC);

	return bmp;
}

//
// http://faithlife.codes/blog/2008/09/displaying_a_splash_screen_with_c_introduction/
//
void HudWindow::SetBitmap(HBITMAP hbmpSplash)
{
	// get the size of the bitmap
	BITMAP bm;
	GetObject(hbmpSplash, sizeof(bm), &bm);
	SIZE sizeSplash = { bm.bmWidth, bm.bmHeight };

	// get the primary monitor's info
	POINT ptZero = { 0 };
	HMONITOR hmonPrimary = MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO monitorinfo = { 0 };
	monitorinfo.cbSize = sizeof(monitorinfo);
	GetMonitorInfo(hmonPrimary, &monitorinfo);

	// center the splash screen in the middle of the primary work area
	const RECT & rcWork = monitorinfo.rcWork;
	POINT ptOrigin;
	ptOrigin.x = rcWork.left + (rcWork.right - rcWork.left - sizeSplash.cx) / 2;
	ptOrigin.y = rcWork.top + (rcWork.bottom - rcWork.top - sizeSplash.cy) / 2;

	// create a memory DC holding the splash bitmap
	HDC hdcScreen = GetDC(HWND_DESKTOP);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcMem, hbmpSplash);

	// use the source image's alpha channel for blending
	BLENDFUNCTION blend = { AC_SRC_OVER, 0, abs(0.9 * 255), AC_SRC_ALPHA };

	// paint the window (in the right location) with the alpha-blended bitmap
	UpdateLayeredWindow(this->hWnd, hdcScreen, &ptOrigin, &sizeSplash,
		hdcMem, &ptZero, this->transparent, &blend, ULW_COLORKEY);

	// delete temporary objects
	SelectObject(hdcMem, hbmpOld);
	DeleteDC(hdcMem);
	ReleaseDC(NULL, hdcScreen);
}