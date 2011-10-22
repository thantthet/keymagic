//Copyright (C) 2008  KeyMagic Project
//http://keymagic.googlecode.com
//
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include <windows.h>
#include <gdiplus.h>
#include "KeyMagic.h"

enum RightMenuCmd{
	RMCMD_EXIT=100,
	RMCMD_MANAGE,
	RMCMD_OSK,
	RMCMD_STARTUP,
	RMCMD_ABOUT
};

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
VOID				OnCreate(HWND hWnd, LPCREATESTRUCT lpcs);
VOID				SetKbData(HWND hWnd);
VOID				DeleteDlgData(HWND hWnd);
BOOL				OpenDialog(HWND hwnd, TCHAR* szFileName,DWORD nMaxFile);
BOOL				RemoveKeyBoard();
BOOL				DeleteKeyFile();
BOOL				AddKeyBoardToList(HWND hWnd, TCHAR* szFileName);
VOID				GetHotKey(WORD wHotkey, LPTSTR ShortCutDisplay);
BOOL				UpdateDlgData(HWND hWnd);
VOID				restart(HWND hWnd);
VOID				error(LPCTSTR lpszFunction) ;
VOID				GetKeyBoards();
VOID				SetStartup(BOOL isEnable);
VOID				OnSize(WPARAM wParam, LPARAM lParam);
VOID				onPaint(HWND hWnd);
VOID				ChangeNotifyIcon(HWND hWnd, HICON hIcon);
VOID				ShowNotifyIcon(HWND hWnd, LPCTSTR szTip, LPCTSTR lpIconName);
VOID				DeleteNotifyIcon(HWND hWnd);
VOID				ShowBallonTip(HWND hWnd, LPCTSTR szInfoTitle, LPCTSTR szInfo, DWORD dwInfoFlags);

extern vector<std::wstring> szFileToDelete;
//extern int cbFileToDelete;
extern int kbindex;
extern HWND hList;