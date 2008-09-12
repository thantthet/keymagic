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
#include "keyMagic.h"

enum RightMenuCmd{
	RMCMD_EXIT=100,
	RMCMD_MANAGE,
	RMCMD_STARTUP,
	RMCMD_ABOUT
};

INT_PTR CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void				OnInitDlg(HWND hWnd);
void				SetKbData(HWND hWnd);
void				DeleteDlgData(HWND hWnd);
bool				OpenDialog(HWND hwnd, char* szFileName,DWORD nMaxFile);
bool				RemoveKeyBoard();
bool				DeleteKeyFile();
bool				AddKeyBoardToList(HWND hWnd, char* szFileName);
void				GetHotKey(WORD wHotkey, LPSTR ShortCutDisplay);
bool				UpdateDlgData(HWND hWnd);
void				restart(HWND hWnd);
void				error(LPCSTR lpszFunction) ;
void				GetKeyBoards();
void				Startup(BOOL isEnable);

extern strDelete *szFileToDelete;
extern int cbFileToDelete;
extern int kbindex;
extern HWND hList;