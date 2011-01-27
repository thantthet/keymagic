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

#define CCH_MAXITEMTEXT 256
#define MENU_FONT L"Parabaik"

#ifndef MMENU_LEFT_PADDING
#define MMENU_LEFT_PADDING 24
#endif

// Structure associated with menu items 
 
typedef struct tagMYITEM 
{ 
	Gdiplus::Bitmap Icon;
    int cchItemText;
	int textWidth;
	int hotkeyWidth;
	TCHAR szItemText[1];
} MYITEM, NEAR *PMYITEM, FAR *LPMYITEM;

void CreateMyMenu(HMENU hMenu);
void DestroyMyMenu(HMENU hMenu);
void DrawMyMenu(LPDRAWITEMSTRUCT lpdis);
void OnMenuMeasure(HWND hwnd, LPMEASUREITEMSTRUCT lpmis);
INT_PTR CALLBACK MyMenuProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool GradientFillRoundRect(HDC hDC, RECT rcItem, int w, int h, DWORD rgb1, DWORD rgb2, DWORD rgb3, ULONG ulMode);
int GetHotkeyX(HWND hwnd);