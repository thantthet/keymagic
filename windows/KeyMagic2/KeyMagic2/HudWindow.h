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

	static constexpr LPTSTR szWindowClass = _T("HudWindow");
	static BOOL registered;

	void onKeyboardDidChange();
	HBITMAP CreateBitmap();
	void SetBitmap(HBITMAP hbmp);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};