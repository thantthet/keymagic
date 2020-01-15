#pragma once

#include "stdafx.h"
#include "Dialog.h"
#include "Resource.h"

class AboutDialog : public Dialog
{
	friend Dialog;

public:
	AboutDialog(HWND hWnd) : Dialog(hWnd) {};

	INT_PTR Show() {
		return DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, Procedure<AboutDialog>);
	}
protected:
	INT_PTR CALLBACK Dispatch(UINT message, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);
		switch (message)
		{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hWnd, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
		}
		return (INT_PTR)FALSE;
	}
};
