#pragma once

#include "Constants.h"
#include "KeyboardManager.h"

class HudWindow
{
public:
	HudWindow(HINSTANCE hInstance, KeyboardManager * manager);
	~HudWindow();

	static ATOM RegisterWindowClass(HINSTANCE hInstance);
	BOOL InitInstance();
	void Show();
	void Hide();

private:
	HINSTANCE hInst;
	HWND hWnd;
	KeyboardManager * kbdManager;

	COLORREF transparentColor = RGB(0xff, 0x0, 0);
	COLORREF textColor = RGB(0xff, 0xff, 0xff);
	static constexpr LPTSTR szWindowClass = _T("HudWindow");
	static BOOL registered;

	void onKeyboardDidChange();
	HBITMAP CreateBitmap();
	void SetBitmapAlpha(HDC hdc, HBITMAP hBitmap);
	void SetBitmap(HBITMAP hbmp);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};