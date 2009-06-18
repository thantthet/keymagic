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
#pragma once

#include "resource.h"

#include <Userenv.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <shellapi.h>
#include <Commdlg.h>
#include <gdiplus.h>

#include <vector>

using namespace std;

//Custom message IDs
#define TRAY_ID 100
#define WM_TRAY WM_USER + 10
#define IDKM_NORMAL 500
#define IDKM_ID 501

#define KM_MESSAGE WM_USER+5000
#define KM_SETKBID KM_MESSAGE+1
#define KM_KILLFOCUS KM_MESSAGE+2
#define KM_GETFOCUS KM_MESSAGE+3
#define KM_RESCAN KM_MESSAGE+3
#define CCH_MAXITEMTEXT 256
#define MAX_LOADSTRING 100

extern char szINIFile[MAX_PATH];
extern char szCurDir[MAX_PATH];

extern void	GetKeyBoards();
extern void	SetHook (HWND hwnd);

struct KeyFileData{
	bool isNew;
	char Name[30];
	char Display[30];
	char Path[MAX_PATH];
	WORD wHotkey;
};

struct strDelete{
	char Path[MAX_PATH];
};

// Structure associated with menu items 
 
typedef struct tagMYITEM 
{ 
    int   cchItemText; 
    char  szItemText[1];
} MYITEM, NEAR *PMYITEM, FAR *LPMYITEM;

INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void				SetHook (HWND hwnd);
void				UnHook ();
BOOL				AddKeyBoard(char* lpKBPath);
BOOL				WorkOnCommand(LPTSTR lpCmdLine);
BOOL				IsAdmin();

extern BOOL bAdmin;

extern TCHAR szKBP[];
extern TCHAR szMS[];
extern TCHAR szSC[];
extern TCHAR szNeedRestart[];
extern TCHAR szKeymagic[];

extern HINSTANCE hInst;								// current instance
extern TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
extern HMENU hKeyMenu;
extern UINT KeyBoardNum;