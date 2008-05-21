//Like a operator of Keymagic.
//Copyright (C) 2008  www.mmgeeks.com
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

#include "stdafx.h"
#include "KeyMagic.h"
#include "shellapi.h"

#include "../KeyMagicDll/KeyMagicDll.h"

#define MAX_LOADSTRING 100

//Custom message IDs
#define WM_TRAY (WM_USER + 1)
#define TRAY_ID 100
#define IDKM_NORMAL (WM_USER + 2)
#define IDKM_ID (WM_USER + 3)

UINT KM_SETKBID;
UINT KM_KILLFOCUS;
HWND LastHWND;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HHOOK hKH;
HHOOK hWPH;
HHOOK hGM;
UINT KeyBoardNum;
HMENU hKeyMenu;
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
VOID				SetHook (HWND hwnd);
VOID				UnHook ();
void				GetKeyBoards();


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_KEYMAGIC, szWindowClass, MAX_LOADSTRING);
	HWND wnd = FindWindow(szWindowClass, szTitle);
	if (wnd) {
		/*ShowWindow(wnd, SW_SHOW);
		ShowWindow(wnd, SW_RESTORE);*/
		return FALSE;
	}

	MyRegisterClass(hInstance);

	GetKeyBoards();

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KEYMAGIC));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KEYMAGIC));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_KEYMAGIC);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_KEYMAGIC));

	return RegisterClassEx(&wcex);
}

void GetKeyBoards(){

	char szCurDir[MAX_PATH];
	char szMenuDisplay[500];
	char szKBNames[500];
	char szKBP[]="KeyBoardPaths";
	char szMS[]="MenuDisplays";

	hKeyMenu = CreatePopupMenu();
	if (!hKeyMenu)
		return;

	GetCurrentDirectory(MAX_PATH, (LPSTR)szCurDir);
	lstrcat(szCurDir, "\\KeyMagic.ini");
	GetPrivateProfileString(szKBP, NULL, NULL, (LPSTR)szKBNames, 500, szCurDir);

	AppendMenu(hKeyMenu, NULL, IDKM_NORMAL, "Disable KeyMagic");
	for (int i=0,Length = lstrlen(&szKBNames[i]);
		Length > 0; 
		i+=Length+1, Length = lstrlen(&szKBNames[i])){
			GetPrivateProfileString(szMS, (LPCSTR)&szKBNames[i], NULL, (LPSTR)szMenuDisplay, 50, szCurDir);
			AppendMenu(hKeyMenu, NULL, IDKM_ID+KeyBoardNum, (LPCSTR)&szMenuDisplay);
			KeyBoardNum++;
	}
};
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   //ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   //if (hKeyMenu){
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWnd;
	nid.uID = TRAY_ID;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.uCallbackMessage = WM_TRAY;
	nid.hIcon = (HICON)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KEYMAGIC));
	lstrcpy(nid.szTip, _T("KeyMagic"));
	Shell_NotifyIcon(NIM_ADD,&nid);
	UpdateWindow(hWnd);
   //}

   SetHook(hWnd);
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HMENU hMenu, hPopUp;
	POINT pt;

	int wmId, wmEvent;

	if (message == KM_KILLFOCUS){
		LastHWND = (HWND)lParam;
		return 0;
	}

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDKM_NORMAL:
			SendMessage(LastHWND, KM_SETKBID, 0, 0);
			break;
		default:
			if (wmId >= IDKM_ID && wmId <= IDKM_ID + KeyBoardNum)
				SendMessage(LastHWND, KM_SETKBID, wmId-IDKM_ID, 1);
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_TRAY:

		if (lParam==WM_LBUTTONDOWN) {

			GetCursorPos(&pt);

			SetForegroundWindow(hWnd);

			TrackPopupMenu(hKeyMenu, TPM_RIGHTBUTTON | TPM_LEFTBUTTON,
				pt.x, pt.y, 0, hWnd, NULL);

		}

		else if (lParam==WM_RBUTTONDOWN) {
			hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDC_KEYMAGIC));
			hPopUp = GetSubMenu(hMenu, 0);

			GetCursorPos(&pt);

			SetForegroundWindow(hWnd);

			TrackPopupMenu(hPopUp, TPM_RIGHTBUTTON | TPM_LEFTBUTTON,
				pt.x, pt.y, 0, hWnd, NULL);
		}
		break;

	case WM_CLOSE:
		ShowWindow(hWnd, SW_HIDE);
		break;

	case WM_DESTROY:
		UnHook();

		NOTIFYICONDATA nid;
		nid.cbSize = sizeof(NOTIFYICONDATA);
		nid.hWnd = hWnd;
		nid.uID = TRAY_ID;
		Shell_NotifyIcon(NIM_DELETE, &nid);

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

VOID SetHook (HWND hwnd)
{
	char szCurDir[MAX_PATH];

	HMODULE hMod = LoadLibrary("KeyMagicDll.dll");
	if (hMod == NULL)   {
			MessageBox(hwnd, "Log", "Error: cannot start dll, KeyMagicDll.dll not found.", 0);
			return ;
	}

	hKH = SetWindowsHookEx(WH_KEYBOARD, &HookKeyProc, hMod, NULL);

	hWPH = SetWindowsHookEx(WH_CALLWNDPROC, &HookWndProc, hMod, NULL);

	hGM = SetWindowsHookEx(WH_GETMESSAGE, &HookGetMsgProc, hMod, NULL);

	KM_SETKBID = RegisterWindowMessage("KM_SETKBID");

	KM_KILLFOCUS = RegisterWindowMessage("KM_KILLFOCUS");
	
	GetCurrentDirectory(MAX_PATH, (LPSTR)szCurDir);

	HookInit(hwnd,hKH, hWPH, hGM, KM_SETKBID, KM_KILLFOCUS, szCurDir);
}

VOID UnHook ()
{
	UnhookWindowsHookEx(hKH);
	UnhookWindowsHookEx(hWPH);
	UnhookWindowsHookEx(hGM);
}