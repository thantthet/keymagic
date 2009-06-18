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
	RMCMD_STARTUP,
	RMCMD_ABOUT
};

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
VOID				OnCreate(HWND hWnd, LPCREATESTRUCT lpcs);
VOID				SetKbData(HWND hWnd);
VOID				DeleteDlgData(HWND hWnd);
BOOL				OpenDialog(HWND hwnd, char* szFileName,DWORD nMaxFile);
BOOL				RemoveKeyBoard();
BOOL				DeleteKeyFile();
BOOL				AddKeyBoardToList(HWND hWnd, char* szFileName);
VOID				GetHotKey(WORD wHotkey, LPSTR ShortCutDisplay);
BOOL				UpdateDlgData(HWND hWnd);
VOID				restart(HWND hWnd);
VOID				error(LPCSTR lpszFunction) ;
VOID				GetKeyBoards();
VOID				SetStartup(BOOL isEnable);
VOID				OnSize(WPARAM wParam, LPARAM lParam);
VOID				onPaint(HWND hWnd);
VOID				ShowNotifyIcon(HWND hWnd, LPCSTR szTip, LPCSTR lpIconName);

extern vector<char*> szFileToDelete;
//extern int cbFileToDelete;
extern int kbindex;
extern HWND hList;