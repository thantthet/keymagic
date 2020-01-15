#pragma once

#include "stdafx.h"

#include <map>

#include "Resource.h"
#include "HotkeyManager.h"

class HotkeysDialog
{
public:
	HotkeysDialog(HWND hWnd) : hWnd(hWnd) {};
	~HotkeysDialog() {};

	INT_PTR Show() {
		return DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_HOTKEYS), hWnd, Procedure);
	}

private:
	HWND hWnd;

	void InitHotkeyToRadioStates(HWND hDlg)
	{
		std::map<UINT, WORD> OnOffControls = {
			{ IDC_RO_ONOFF_SC, MAKEWORD(0, HOTKEYF_SHIFT | HOTKEYF_CONTROL) },
			{ IDC_RO_ONOFF_CS, MAKEWORD(VK_SPACE, HOTKEYF_CONTROL) },
			{ IDC_RO_ONOFF_WS, MAKEWORD(VK_SPACE, HOTKEYF_EXT) }
		};
		std::map<UINT, WORD> NextControls = {
			{ IDC_RO_NEXT_SC, MAKEWORD(0, HOTKEYF_SHIFT | HOTKEYF_CONTROL) },
			{ IDC_RO_NEXT_CS, MAKEWORD(VK_SPACE, HOTKEYF_CONTROL) },
			{ IDC_RO_NEXT_WS, MAKEWORD(VK_SPACE, HOTKEYF_EXT) }
		};

		HotkeyManager* hmgr = HotkeyManager::sharedManager();

		bool checked = false;

		for (auto& pair : OnOffControls)
		{
			if (pair.second == hmgr->hky_onoff)
			{
				CheckDlgButton(hDlg, pair.first, true);
				checked = true;
			}
		}

		if (checked == false)
		{
			CheckDlgButton(hDlg, IDC_RO_ONOFF_CUSTOM, MF_CHECKED);
			SendMessage(GetDlgItem(hDlg, IDC_HK_ONOFF), HKM_SETHOTKEY, hmgr->hky_onoff, 0);
		}

		checked = false;

		for (auto& pair : NextControls)
		{
			if (pair.second == hmgr->hky_nextkbd)
			{
				CheckDlgButton(hDlg, pair.first, true);
				checked = true;
			}
		}

		if (checked == false)
		{
			CheckDlgButton(hDlg, IDC_RO_NEXT_CUSTOM, MF_CHECKED);
			EnableWindow(GetDlgItem(hDlg, IDC_HK_NEXT), true);
			SendMessage(GetDlgItem(hDlg, IDC_HK_NEXT), HKM_SETHOTKEY, hmgr->hky_nextkbd, 0);
		}
	}

	void ReloadRadioButtonStates(HWND hDlg)
	{
		bool isChecked;

		// enable/disable custom hotkey control
		isChecked = IsDlgButtonChecked(hDlg, IDC_RO_ONOFF_CUSTOM);
		EnableWindow(GetDlgItem(hDlg, IDC_HK_ONOFF), isChecked);

		isChecked = IsDlgButtonChecked(hDlg, IDC_RO_NEXT_CUSTOM);
		EnableWindow(GetDlgItem(hDlg, IDC_HK_NEXT), isChecked);

		// get state
		isChecked = IsDlgButtonChecked(hDlg, IDC_RO_ONOFF_SC);
		EnableWindow(GetDlgItem(hDlg, IDC_RO_NEXT_SC), !isChecked);
		if (isChecked) CheckDlgButton(hDlg, IDC_RO_NEXT_SC, false);

		isChecked = IsDlgButtonChecked(hDlg, IDC_RO_ONOFF_CS);
		EnableWindow(GetDlgItem(hDlg, IDC_RO_NEXT_CS), !isChecked);
		if (isChecked) CheckDlgButton(hDlg, IDC_RO_NEXT_CS, false);

		isChecked = IsDlgButtonChecked(hDlg, IDC_RO_ONOFF_WS);
		EnableWindow(GetDlgItem(hDlg, IDC_RO_NEXT_WS), !isChecked);
		if (isChecked) CheckDlgButton(hDlg, IDC_RO_NEXT_WS, false);
	}

	void SaveHotkeys(HWND hDlg)
	{
		bool isChecked;

		WORD wOnOff = 0, wNext = 0;

		std::map<UINT, WORD> OnOffControls = {
			{IDC_RO_ONOFF_SC, MAKEWORD(0, HOTKEYF_SHIFT | HOTKEYF_CONTROL) },
			{IDC_RO_ONOFF_CS, MAKEWORD(VK_SPACE, HOTKEYF_CONTROL) },
			{IDC_RO_ONOFF_WS, MAKEWORD(VK_SPACE, HOTKEYF_EXT) }
		};
		std::map<UINT, WORD> NextControls = {
			{ IDC_RO_NEXT_SC, MAKEWORD(0, HOTKEYF_SHIFT | HOTKEYF_CONTROL) },
			{ IDC_RO_NEXT_CS, MAKEWORD(VK_SPACE, HOTKEYF_CONTROL) },
			{ IDC_RO_NEXT_WS, MAKEWORD(VK_SPACE, HOTKEYF_EXT) }
		};

		isChecked = IsDlgButtonChecked(hDlg, IDC_RO_ONOFF_CUSTOM);
		if (isChecked)
		{
			wOnOff = (WORD)SendMessage(GetDlgItem(hDlg, IDC_HK_ONOFF), HKM_GETHOTKEY, 0, 0);
		}
		else {
			for (auto& pair : OnOffControls)
			{
				isChecked = IsDlgButtonChecked(hDlg, pair.first);
				if (isChecked)
				{
					wOnOff = pair.second;
				}
			}
		}

		isChecked = IsDlgButtonChecked(hDlg, IDC_RO_NEXT_CUSTOM);
		if (isChecked)
		{
			wNext = (WORD)SendMessage(GetDlgItem(hDlg, IDC_HK_NEXT), HKM_GETHOTKEY, 0, 0);
		}
		else {
			for (auto& pair : NextControls)
			{
				isChecked = IsDlgButtonChecked(hDlg, pair.first);
				if (isChecked)
				{
					wNext = pair.second;
				}
			}
		}

		HotkeyManager* hmgr = HotkeyManager::sharedManager();
		hmgr->hky_onoff = wOnOff;
		hmgr->hky_nextkbd = wNext;
		hmgr->WriteHotkey();
		//SetHotKeyTexts();
	}

	INT_PTR CALLBACK Dispatch(UINT message, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);
		switch (message)
		{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_ACTIVATE:
			InitHotkeyToRadioStates(hWnd);
			ReloadRadioButtonStates(hWnd);
			break;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK)
			{
				SaveHotkeys(hWnd);
				EndDialog(hWnd, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			else if (LOWORD(wParam) == IDCANCEL) {
				EndDialog(hWnd, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			else {
				ReloadRadioButtonStates(hWnd);
			}
			break;
		}
		return (INT_PTR)FALSE;
	}

	static LRESULT CALLBACK Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		try {
			if (auto window = GetWindowLongPtr(hWnd, GWLP_USERDATA)) {
				return reinterpret_cast <HotkeysDialog*> (window)->Dispatch(message, wParam, lParam);
			}
			else {
				switch (message) {
				case WM_INITDIALOG:
					if (auto window = new (std::nothrow) HotkeysDialog(hWnd)) {
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