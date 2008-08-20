//Like a operator of Keymagic.
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

#include "MyMenu.h"
#include "MyButton.h"
#include "DlgProc.h"
#include "DllUnload.h"
#include "../KeyMagicDll/KeyMagicDll.h"


TCHAR szKBP[]="KeyBoardPaths";
TCHAR szMS[]="MenuDisplays";
TCHAR szSC[]="ShortCuts";
TCHAR szNeedRestart[] = "Application needs to restart to work correctly";
TCHAR szKeymagic[] = "Keymagic";

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text

#pragma data_seg(".keymagic")
HHOOK hKH = NULL;
HHOOK hWPH = NULL;
HHOOK hGM = NULL;
#pragma data_seg()

//Make sure that section can READ WRITE and SHARE
#pragma comment(linker, "/SECTION:.keymagic,RWS")

bool hide = false;
HMENU hKeyMenu;
char szINIFile[MAX_PATH];
char szCurDir[MAX_PATH];

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);

	hInst = hInstance;

	GetModuleFileName(hInst, szCurDir, MAX_PATH);

	int i;
	for (i=lstrlen(szCurDir); szCurDir[i] != '\\'; i--){
	}
	szCurDir[i] = NULL;

	lstrcpy(szINIFile,szCurDir);

	PathAppend(szINIFile, "KeyMagic.ini");

	if (WorkOnCommand(lpCmdLine))
		return 0;

	if (OpenMutexW(SYNCHRONIZE, NULL, L"\u1000\u102E\u1038\u1019\u1000\u1039\u1002\u103A\u1005\u1039")){
		HWND prevhandle = FindWindow(NULL, "KeyMagic 1.2");
		ShowWindow(prevhandle, SW_SHOW);
		ExitProcess(0);
	}

	HANDLE MtxHANDLE= CreateMutexW(NULL, TRUE, L"\u1000\u102E\u1038\u1019\u1000\u1039\u1002\u103A\u1005\u1039");

	DialogBox(hInst, MAKEINTRESOURCE(IDD_MAINBOX), NULL, WndProc);

	ReleaseMutex(MtxHANDLE);
	return (int) 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		char* buildTime[50];
		lstrcpyA((LPSTR)buildTime,__DATE__);
		lstrcatA((LPSTR)buildTime," - ");
		lstrcatA((LPSTR)buildTime,__TIME__);
		SendDlgItemMessageA(hDlg, IDC_COMPLIE, WM_SETTEXT, 0, (LPARAM)buildTime);

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

	HMODULE hMod = LoadLibrary("KeyMagicDll.dll");
	if (hMod == NULL)   {
			MessageBox(hwnd, "Log", "Error: cannot start dll, KeyMagicDll.dll not found.", 0);
			return ;
	}

	hKH = SetWindowsHookEx(WH_KEYBOARD, &HookKeyProc, hMod, NULL);

	hWPH = SetWindowsHookEx(WH_CALLWNDPROC, &HookWndProc, hMod, NULL);

	hGM = SetWindowsHookEx(WH_GETMESSAGE, &HookGetMsgProc, hMod, NULL);

	HookInit(hwnd,hKH, hWPH, hGM, szCurDir);
}

VOID UnHook ()
{
	UnhookWindowsHookEx(hKH);
	UnhookWindowsHookEx(hWPH);
	UnhookWindowsHookEx(hGM);
}

bool WorkOnCommand(LPTSTR lpCmdLine){

	if (!lstrlen(lpCmdLine)){
		return false;
	}
	if (lpCmdLine[0] == '-'){
		switch (lpCmdLine[1]){
		case 'i':
			if (AddKeyBoard(&lpCmdLine[3]))
				MessageBox(GetDesktopWindow(), "The keyboard has been successfully added.", "KeyMagic", MB_ICONINFORMATION | MB_OK);
			return true;
		case 's':
			hide = true;
			return false;
		case 'u':
			FreeLibrary(LoadLibrary("KeymagicDll.dll"));
			UnHook();
			Scanner();
			return true;
		}
	}
	return false;
}

bool AddKeyBoard(char* lpKBPath){
	char lpPath[MAX_PATH];
	char lpName[MAX_PATH];
	char szKBPath[MAX_PATH];
	char szKBP[] = "KeyBoardPaths";
	char szMD[] = "MenuDisplays";
	char szSC[] = "ShortCuts";
	
	GetFileTitle(lpKBPath, lpName, MAX_PATH);

	if (lpName [lstrlen(lpName)-4] != '.')
		lstrcat(lpName, ".kmk");

	wsprintf(lpPath, "KeyBoards\\%s", lpName);

	lpName [lstrlen(lpName)-4] = NULL;

	//if (SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, szKBPath))
	//	return false;

	lstrcpy(szKBPath, szCurDir);

	for (int i=lstrlen(szKBPath); szKBPath[i] != '\\'; i--){
		szKBPath[i] = NULL;
		if (szKBPath[i-1] == '\\'){
			szKBPath[i-1] = NULL;
			break;
		}
	}
	//PathAppend(szKBPath, "KeyMagic");
	PathAppend(szKBPath, lpPath);

	if (!CopyFile(lpKBPath, szKBPath, false)){
		MessageBox(GetDesktopWindow(), "File copying fail!", szKeymagic, MB_ICONERROR);
		return false;
	}

	if (!WritePrivateProfileString(szKBP, lpName, lpPath, szINIFile)){
		return false;
	}
	if (!WritePrivateProfileString(szMD, lpName, lpName, szINIFile)){
		return false;
	}
	if (!WritePrivateProfileString(szSC, lpName, "0", szINIFile)){
		return false;
	}

	return true;
}