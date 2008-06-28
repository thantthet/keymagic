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
#include "../KeyMagicDll/KeyMagicDll.h"

HWND LastHWND;
TCHAR szKBP[]="KeyBoardPaths";
TCHAR szMS[]="MenuDisplays";
TCHAR szSC[]="ShortCuts";
TCHAR szNeedRestart[] = "Application needs to restart to work correctly";
TCHAR szKeymagic[] = "Keymagic";

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text

HHOOK hKH;
HHOOK hWPH;
HHOOK hGM;
bool hide = false;
// Forward declarations of functions included in this code module:
INT_PTR CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void				SetHook (HWND hwnd);
void				UnHook ();
void				GetKeyBoards();
bool				AddKeyBoard(char* lpKBPath);
bool				WorkOnCommand(LPTSTR lpCmdLine);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);

	hInst = hInstance;

	if (WorkOnCommand(lpCmdLine))
		return 0;

	if (OpenMutexW(SYNCHRONIZE, NULL, L"\u1000\u102E\u1038\u1019\u1000\u1039\u1002\u103A\u1005\u1039")){
		HWND prevhandle = FindWindow(NULL, "Keymagic");
		ShowWindow(prevhandle, SW_SHOW);
		ExitProcess(0);
	}

	HANDLE MtxHANDLE= CreateMutexW(NULL, TRUE, L"\u1000\u102E\u1038\u1019\u1000\u1039\u1002\u103A\u1005\u1039");

	DialogBox(hInst, MAKEINTRESOURCE(IDD_MAINBOX), NULL, WndProc);

	ReleaseMutex(MtxHANDLE);
	return (int) 0;
}

void GetKeyBoards(){

	char szCurDir[MAX_PATH];
	char szMenuDisplay[MAX_PATH];
	char szKBNames[500];
	char szShortCut[20];
	WORD wHotKey;

	if (hKeyMenu)
		DestroyMenu(hKeyMenu);

	hKeyMenu = CreatePopupMenu();
	if (!hKeyMenu)
		return;

	GetModuleFileName(hInst, szCurDir, MAX_PATH);
	int i;
	for (i=lstrlen(szCurDir); szCurDir[i] != '\\'; i--){
	}
	szCurDir[i] = NULL;

	PathAppend(szCurDir, "KeyMagic.ini");
	GetPrivateProfileString(szKBP, NULL, NULL, (LPSTR)szKBNames, 500, szCurDir);

	GetPrivateProfileString(szMS, &szKBNames[0], NULL, (LPSTR)szMenuDisplay, MAX_PATH, szCurDir);
	wHotKey = GetPrivateProfileInt(szSC, &szKBNames[0], 0, szCurDir);
	lstrcat(szMenuDisplay, "\t");

	GetHotKey(wHotKey, szShortCut);
	lstrcat(szMenuDisplay, szShortCut);

	AppendMenu(hKeyMenu, NULL, IDKM_NORMAL, szMenuDisplay);
	CreateMyMenu(hKeyMenu, IDKM_NORMAL);
	KeyBoardNum=0;
	for (int i=lstrlen(&szKBNames[0])+1,Length = lstrlen(&szKBNames[i]);
		Length > 0; 
		i+=Length+1, Length = lstrlen(&szKBNames[i])){
			GetPrivateProfileString(szMS, &szKBNames[i], NULL, szMenuDisplay, MAX_PATH, szCurDir);
			wHotKey = GetPrivateProfileInt(szSC, &szKBNames[i], 0, szCurDir);

			lstrcat(szMenuDisplay, "\t");
			GetHotKey(wHotKey, szShortCut);
			lstrcat(szMenuDisplay, szShortCut);

			AppendMenu(hKeyMenu, NULL, IDKM_ID+KeyBoardNum, szMenuDisplay);
			CreateMyMenu(hKeyMenu, IDKM_ID+KeyBoardNum);
			KeyBoardNum++;
	}

	CheckMenuRadioItem(hKeyMenu, IDKM_NORMAL, 
		KeyBoardNum + IDKM_ID, 
		IDKM_NORMAL,
		MF_BYCOMMAND);

};

INT_PTR CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HMENU hMenu;
	POINT pt;
	int wmId, wmEvent;
	KeyFileData *Data;

	switch (message)
	{
	case KM_KILLFOCUS:
		LastHWND = (HWND)lParam;
		break;

	case KM_GETFOCUS:
		CheckMenuRadioItem(hKeyMenu, IDKM_NORMAL, 
			KeyBoardNum + IDKM_ID , 
			wParam + IDKM_NORMAL, 
			MF_BYCOMMAND);
		SendMessage((HWND)lParam, KM_RESCAN, 0, 0);
		break;

	case WM_INITDIALOG:
		OnInitDlg(hWnd);
		break;

	case WM_TIMER:
		if (wParam == 100){
			ShowWindow(hWnd, SW_HIDE);
			hide = false;
			KillTimer(hWnd, 100);
		}
		break;

	case WM_SYSCOMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		}
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId){
		case IDC_APPLY:
			if (kbindex == -1)
				goto Loc1;

			Data = (KeyFileData*)SendMessage(hList, LB_GETITEMDATA, kbindex, 0);
			SendDlgItemMessage(hWnd, IDC_DISPLAY, WM_GETTEXT, 30, (LPARAM)Data->Display);
			Data->wHotkey = (WORD)SendDlgItemMessage(hWnd, IDC_SHORTCUT, HKM_GETHOTKEY, 0, 0);
			SendDlgItemMessage(hWnd, IDC_PATH, WM_GETTEXT, MAX_PATH, (LPARAM)Data->Path);
			Loc1:
			SetKbData(hWnd);
			DeleteKeyFile();
			restart(hWnd);

			SendMessage(LastHWND, KM_RESCAN, 0, 0);
			break;

		case IDC_DONE:
			if (kbindex == -1)
				goto Loc2;

			hList = GetDlgItem(hWnd, IDC_KEYBOARDS);

			Data = (KeyFileData*)SendMessage(hList, LB_GETITEMDATA, kbindex, 0);
			SendDlgItemMessage(hWnd, IDC_DISPLAY, WM_GETTEXT, 30, (LPARAM)Data->Display);
			Data->wHotkey = (WORD)SendDlgItemMessage(hWnd, IDC_SHORTCUT, HKM_GETHOTKEY, 0, 0);
			SendDlgItemMessage(hWnd, IDC_PATH, WM_GETTEXT, MAX_PATH, (LPARAM)Data->Path);
			Loc2:
			SetKbData(hWnd);

			ShowWindow(hWnd, SW_HIDE);
			DeleteKeyFile();
			restart(hWnd);

			SendMessage(LastHWND, KM_RESCAN, 0, 0);
			break;

		case IDC_CANCEL:
			restart(hWnd);
			ShowWindow(hWnd, SW_HIDE);
			break;

		case IDC_KEYBOARDS:
			if (wmEvent == LBN_SELCHANGE){

				CheckDlgButton(hWnd, IDC_DIR, false);
				EnableWindow(GetDlgItem(hWnd,IDC_PATH), false);
				if (kbindex == -1)
					goto next;
				
				Data = (KeyFileData*)SendMessage(hList, LB_GETITEMDATA, kbindex, 0);
				if (!Data)
					break;

				SendDlgItemMessage(hWnd, IDC_DISPLAY, WM_GETTEXT, 30, (LPARAM)Data->Display);
				Data->wHotkey = (WORD)SendDlgItemMessage(hWnd, IDC_SHORTCUT, HKM_GETHOTKEY, 0, 0);
				SendDlgItemMessage(hWnd, IDC_PATH, WM_GETTEXT, MAX_PATH, (LPARAM)Data->Path);
next:
				kbindex = SendMessage(hList, LB_GETCURSEL, 0, 0);
				if (kbindex == -1)
					break;
				
				Data = (KeyFileData*)SendMessage(hList, LB_GETITEMDATA, kbindex, 0);
				if (!Data)
					break;
				SendDlgItemMessage(hWnd, IDC_DISPLAY, WM_SETTEXT, 0, (LPARAM)Data->Display);
				SendDlgItemMessage(hWnd, IDC_SHORTCUT, HKM_SETHOTKEY, Data->wHotkey, 0);
				SendDlgItemMessage(hWnd, IDC_PATH, WM_SETTEXT, 0, (LPARAM)Data->Path);
				if (Data->Path[1] == ':'){
					EnableWindow(GetDlgItem(hWnd,IDC_PATH), true);
					CheckDlgButton(hWnd, IDC_DIR, true);
				}
			}
			break;

		case IDC_DIR:
			UINT state;
			state = IsDlgButtonChecked(hWnd, IDC_DIR);
			if (state == BST_CHECKED)
				EnableWindow(GetDlgItem(hWnd,IDC_PATH), true);
			else
				EnableWindow(GetDlgItem(hWnd,IDC_PATH), false);
			break;

		case IDC_ADD:
			char szFileName[MAX_PATH];
			if (!OpenDialog(hWnd, szFileName, MAX_PATH))
				break;

			AddKeyBoardToList(hWnd ,szFileName);
			SendDlgItemMessage(hWnd, IDC_DISPLAY, EM_SETMODIFY, TRUE, 0);
			
			break;

		case IDC_REMOVE:

			RemoveKeyBoard();
			
			break;

		case IDKM_NORMAL:

			SendMessage(LastHWND, KM_SETKBID, -1, 0);
			CheckMenuRadioItem(hKeyMenu, IDKM_NORMAL,
				KeyBoardNum + IDKM_ID, 
				IDKM_NORMAL, 
				MF_BYCOMMAND);

			break;

		default:
			if (wmId >= IDKM_ID && wmId <= IDKM_ID + KeyBoardNum){

				SendMessage(LastHWND, KM_SETKBID, wmId - IDKM_NORMAL, true);

				if (MF_CHECKED & GetMenuState(hKeyMenu, wmId, MF_BYCOMMAND))
					wmId = IDKM_NORMAL;

				CheckMenuRadioItem(hKeyMenu, IDKM_NORMAL,
					KeyBoardNum + IDKM_ID,
					wmId,
					MF_BYCOMMAND);
			}
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
			hMenu = CreatePopupMenu();
			AppendMenu(hMenu, MF_BYCOMMAND, 101, "Key&Magic");
			CreateMyMenu(hMenu, 101);
			AppendMenu(hMenu, MF_BYCOMMAND, 100, "E&xit");
			CreateMyMenu(hMenu, 100);

			GetCursorPos(&pt);

			SetForegroundWindow(hWnd);

			BOOL CMD= TrackPopupMenu(hMenu, TPM_RIGHTBUTTON | TPM_LEFTBUTTON | TPM_RETURNCMD,
				pt.x, pt.y, 0, hWnd, NULL);

			DestroyMyMenu(hMenu, 100);

			DestroyMenu(hMenu);

			if (CMD == 100){
				PostQuitMessage(0);
			}
			else if (CMD == 101){
				ShowWindow(hWnd, SW_SHOW);
			}

			PostMessage(hWnd, WM_NULL, 0, 0);
		}
		break;
	case WM_CTLCOLORDLG:
		return (INT_PTR)CreateSolidBrush(RGB(236, 244, 255));
	//	break;

	case WM_CTLCOLORSTATIC:
		SetBkColor((HDC)wParam, RGB(236, 244, 255));
		return (INT_PTR)CreateSolidBrush(RGB(236, 244, 255));

	case WM_DRAWITEM:

		DrawMyMenu((LPDRAWITEMSTRUCT)lParam);
		DrawMyButton((LPDRAWITEMSTRUCT)lParam);

		break;

	case WM_MEASUREITEM:

		OnMenuMeasure(hWnd,(LPMEASUREITEMSTRUCT)lParam);

		break;

	case WM_CLOSE:

		restart(hWnd);
		ShowWindow(hWnd, SW_HIDE);
		break;

	case WM_DESTROY:

		UnHook();

		NOTIFYICONDATA nid;
		nid.cbSize = sizeof(NOTIFYICONDATA);
		nid.hWnd = hWnd;
		nid.uID = TRAY_ID;
		Shell_NotifyIcon(NIM_DELETE, &nid);

		for (UINT uID = IDKM_NORMAL; uID < IDKM_ID+KeyBoardNum; uID++) 
		{
			DestroyMyMenu(hKeyMenu, uID);
		}

		EndDialog(hWnd,0);

		DeleteDlgData(hWnd);
		break;
	}
	return (INT_PTR)FALSE;
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
	
	GetModuleFileName(hInst, szCurDir, MAX_PATH);
	int i;
	for (i=lstrlen(szCurDir); szCurDir[i] != '\\'; i--){
	}
	szCurDir[i] = NULL;

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
		}
	}
	return false;
}

bool AddKeyBoard(char* lpKBPath){
	char lpPath[MAX_PATH];
	char lpName[MAX_PATH];
	char szCurDir[MAX_PATH];
	char szKBPath[MAX_PATH];
	char szKBP[] = "KeyBoardPaths";
	char szMD[] = "MenuDisplays";
	char szSC[] = "ShortCuts";

	GetModuleFileName(hInst, szCurDir, MAX_PATH);

	int i;
	for (i=lstrlen(szCurDir); szCurDir[i] != '\\'; i--){
	}

	szCurDir[i] = NULL;
	PathAppend(szCurDir, "KeyMagic.ini");
	
	GetFileTitle(lpKBPath, lpName, MAX_PATH);
	wsprintf(lpPath, "KeyBoards\\%s", lpName);
	lpName [lstrlen(lpName)-4] = NULL;

	if (SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, szKBPath))
		return false;

	PathAppend(szKBPath, "KeyMagic");
	PathAppend(szKBPath, lpPath);

	if (!CopyFile(lpKBPath, szKBPath, false)){
		MessageBox(GetDesktopWindow(), "File copying fail!", szKeymagic, MB_ICONERROR);
		return false;
	}

	if (!WritePrivateProfileString(szKBP, lpName, lpPath, szCurDir)){
		return false;
	}
	if (!WritePrivateProfileString(szMD, lpName, lpName, szCurDir)){
		return false;
	}
	if (!WritePrivateProfileString(szSC, lpName, "0", szCurDir)){
		return false;
	}

	return true;
}