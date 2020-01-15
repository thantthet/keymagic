#pragma once

#include "stdafx.h"

class Dialog
{
public:
	Dialog(HWND hWnd) : hWnd(hWnd) {};
	~Dialog() {};

	virtual INT_PTR Show() { return FALSE; }

protected:
	HWND hWnd;

	virtual INT_PTR CALLBACK Dispatch(UINT message, WPARAM wParam, LPARAM lParam) {
		return (INT_PTR)FALSE;
	};

	template <typename T>
	static LRESULT CALLBACK Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		try {
			if (auto window = GetWindowLongPtr(hWnd, GWLP_USERDATA)) {
				return reinterpret_cast <T*> (window)->Dispatch(message, wParam, lParam);
			}
			else {
				switch (message) {
				case WM_INITDIALOG:
					if (auto window = new (std::nothrow) T(hWnd)) {
						SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);
						return window->Dispatch(WM_INITDIALOG, wParam, lParam);
					}
					else
						return FALSE;
				}
				return FALSE;
			}
		}
		catch (...) {
			DestroyWindow(hWnd);
			return 0;
		}
	}
};