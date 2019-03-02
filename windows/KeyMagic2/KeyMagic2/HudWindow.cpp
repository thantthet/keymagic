#include "stdafx.h"
#include "HudWindow.h"
#include "Resource.h"
#include <time.h>

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
		1,            // timer identifier
		3000,
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
		WS_EX_LAYERED | WS_EX_TOPMOST,
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

HBITMAP HudWindow::CreateBitmap()
{
	HDC hdc = GetDC(HWND_DESKTOP);
	return CreateCompatibleBitmap(hdc, 100, 100);
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
	HDC hdcScreen = GetDC(NULL);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmpOld = (HBITMAP)SelectObject(hdcMem, hbmpSplash);

	// use the source image's alpha channel for blending
	BLENDFUNCTION blend = { 0 };
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	// paint the window (in the right location) with the alpha-blended bitmap
	UpdateLayeredWindow(this->hWnd, hdcScreen, &ptOrigin, &sizeSplash,
		hdcMem, &ptZero, RGB(255, 255, 255), &blend, ULW_COLORKEY);

	// delete temporary objects
	SelectObject(hdcMem, hbmpOld);
	DeleteDC(hdcMem);
	ReleaseDC(NULL, hdcScreen);
}