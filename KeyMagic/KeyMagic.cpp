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

#include <Userenv.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <shellapi.h>
#include <Commdlg.h>

#include "KeyMagic.h"
#include "../KeyMagicDll/KeyMagicDll.h"

#define MAX_LOADSTRING 100

//Custom message IDs
#define TRAY_ID 100
#define WM_TRAY (WM_USER + 10)
#define IDKM_NORMAL (WM_USER + 11)
#define IDKM_ID (WM_USER + 12)

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

HMENU hKeyMenu;
UINT KeyBoardNum;
HWND hList;

int kbindex=-1;
bool hide = false;
int cbFileToDelete = 0;

strDelete *szFileToDelete;
// Forward declarations of functions included in this code module:
INT_PTR CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
VOID				SetHook (HWND hwnd);
VOID				UnHook ();
VOID				GetKeyBoards();
VOID				CreateMyMenu(HMENU hMenu,UINT uID);
VOID				DrawMyMenu(LPDRAWITEMSTRUCT lpdis);
VOID				OnMenuMeasure(HWND hwnd, LPMEASUREITEMSTRUCT lpmis);
VOID				OnInitDlg(HWND hWnd);
VOID				GetHotKey(WORD wHotkey, LPSTR ShortCutDisplay);
VOID				DeleteDlgData(HWND hWnd);
VOID				SetKbData(HWND hWnd);
VOID				restart(HWND hWnd);
bool				OpenDialog(HWND hwnd, char* szFileName,DWORD nMaxFile);
bool				UpdateDlgData(HWND hWnd);
bool				AddKeyBoard(char* lpKBPath);
bool				WorkOnCommand(LPTSTR lpCmdLine);
bool				AddKeyBoardToList(HWND hWnd, char* szFileName);
bool				RemoveKeyBoard();
bool				DeleteKeyFile();
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

VOID CreateMyMenu(HMENU hMenu,UINT uID){

	MENUINFO mi;
	MENUITEMINFO mii;
	MYITEM *pMyItem;

	mi.cbSize = sizeof(MENUINFO);
	mi.fMask = MIM_APPLYTOSUBMENUS | MIM_BACKGROUND;
	mi.hbrBack = CreateSolidBrush(RGB(255,255,255));
	SetMenuInfo(hMenu, &mi);

	pMyItem = (MYITEM *) LocalAlloc(LPTR,
		sizeof(MYITEM) + CCH_MAXITEMTEXT);

	// Save the item text in the item structure. 

	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STRING; 
	mii.dwTypeData = pMyItem->szItemText; 
	mii.cch = CCH_MAXITEMTEXT; 
	GetMenuItemInfo(hMenu, uID, FALSE, &mii); 
	pMyItem->cchItemText = mii.cch; 

	// Reallocate the structure to the minimum required size. 

	pMyItem = (MYITEM *) LocalReAlloc(pMyItem,
		sizeof(MYITEM) + mii.cch, LPTR); 

	// Change the item to an owner-drawn item, and save 
	// the address of the item structure as item data. 

	mii.fMask = MIIM_FTYPE | MIIM_DATA; 
	mii.fType = MFT_OWNERDRAW; 
	mii.dwItemData = (ULONG_PTR) pMyItem; 
	SetMenuItemInfo(hMenu, uID, FALSE, &mii); 

}

void DestroyMyMenu(HMENU hMenu, UINT uID){
	MENUITEMINFO mii;
	//UINT uID; 
    MYITEM *pMyItem; 

	//for (uID = IDKM_NORMAL; uID < IDKM_ID+KeyBoardNum; uID++) 
	//{ 
		// Get the item data. 

	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_DATA; 
	GetMenuItemInfo(hKeyMenu, uID, FALSE, &mii); 
	pMyItem = (MYITEM *) mii.dwItemData; 

	// free the item structure. 
	LocalFree(pMyItem); 
	//} 

}

void DrawMyMenu(LPDRAWITEMSTRUCT lpdis){

	UINT cch;
	MENUITEMINFO mii;
	MYITEM *pMyItem = (MYITEM *) lpdis->itemData;
	
	if (lpdis->CtlType == ODT_MENU){

		if (lpdis->itemState & ODS_SELECTED)
		{
			SelectObject(lpdis->hDC,CreateSolidBrush(RGB(210,240,255)));
			SetTextColor(lpdis->hDC, RGB(0,128,170));
			SelectObject(lpdis->hDC,CreatePen(PS_INSIDEFRAME, 1, RGB(153,217,234)));
		}
		else if (lpdis->itemState & ODS_CHECKED)
		{
			SelectObject(lpdis->hDC,CreateSolidBrush(RGB(240,250,255)));
			SetTextColor(lpdis->hDC, RGB(0,128,192));
			SelectObject(lpdis->hDC,CreatePen(PS_INSIDEFRAME, 1, RGB(110,205,220)));
		}
		else
		{
			SelectObject(lpdis->hDC,CreateSolidBrush(RGB(255,255,255)));
			SetTextColor(lpdis->hDC, RGB(10,120,245));
			SelectObject(lpdis->hDC,CreatePen(PS_INSIDEFRAME, 1, RGB(255,255,255)));
		} 
		
		SetBkMode(lpdis->hDC, TRANSPARENT);
		RoundRect(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, lpdis->rcItem.right, lpdis->rcItem.bottom, 3, 3);

		char Temp[256];

		strcpy(Temp, pMyItem->szItemText);

		strtok(Temp, "\t");
		char* szShortCut = strtok(NULL, "\t");

		lpdis->rcItem.top += 3;

		lpdis->rcItem.left += 10;

		DrawText(lpdis->hDC, Temp, lstrlen(Temp), &lpdis->rcItem, DT_VCENTER);

		lpdis->rcItem.right -= 8;
		DrawText(lpdis->hDC, szShortCut, lstrlen(szShortCut), &lpdis->rcItem,DT_VCENTER | DT_RIGHT);

	}
}

void OnMenuMeasure(HWND hwnd,LPMEASUREITEMSTRUCT lpmis)
{

    MYITEM *pMyItem = (MYITEM *) lpmis->itemData;
	HDC hdc = GetDC(hwnd);
    SIZE size; 
 
    GetTextExtentPoint32(hdc, pMyItem->szItemText, 
            pMyItem->cchItemText, &size); 
 
    lpmis->itemWidth = size.cx; 
    lpmis->itemHeight = size.cy+5;

    ReleaseDC(hwnd, hdc);

}

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
		if (hide == true)
			SetTimer(hWnd, 100, USER_TIMER_MINIMUM, NULL);
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

			hList = GetDlgItem(hWnd, IDC_KEYBOARDS);

			Data = (KeyFileData*)SendMessage(hList, LB_GETITEMDATA, kbindex, 0);
			SendDlgItemMessage(hWnd, IDC_DISPLAY, WM_GETTEXT, 30, (LPARAM)Data->Display);
			Data->wHotkey = (WORD)SendDlgItemMessage(hWnd, IDC_SHORTCUT, HKM_GETHOTKEY, 0, 0);
			SendDlgItemMessage(hWnd, IDC_PATH, WM_GETTEXT, MAX_PATH, (LPARAM)Data->Path);
			Loc1:
			SetKbData(hWnd);
			DeleteKeyFile();
			restart(hWnd);
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

			break;

		case IDC_CANCEL:
			restart(hWnd);
			ShowWindow(hWnd, SW_HIDE);
			break;

		case IDC_KEYBOARDS:
			if (wmEvent == LBN_SELCHANGE){
				hList = GetDlgItem(hWnd, IDC_KEYBOARDS);

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

				SendMessage(LastHWND, KM_SETKBID, wmId-IDKM_NORMAL, true);

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

	case WM_DRAWITEM:

		DrawMyMenu((LPDRAWITEMSTRUCT)lParam);

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

void GetHotKey(WORD wHotkey, LPSTR ShortCutDisplay){

	BYTE vkey[1],  modkey;

	vkey[0] = wHotkey;
	modkey = wHotkey >> 8;
	ShortCutDisplay[0] = NULL;

	if (modkey & HOTKEYF_SHIFT){
		lstrcat(ShortCutDisplay, "Shift + ");
	}

	if (modkey & HOTKEYF_CONTROL){
		lstrcat(ShortCutDisplay, "Ctrl + ");
	}

	if (modkey & HOTKEYF_ALT){
		lstrcat(ShortCutDisplay, "Alt + ");
	}

	vkey[1] = 0;

	lstrcat(ShortCutDisplay, (LPCSTR)&vkey[0]);
}

void OnInitDlg(HWND hWnd){
	HMENU hMenu;
	KeyFileData *Data;
	char szKBPath[MAX_PATH];

	if (SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, szKBPath)){
		goto next;
	}
	PathAppend(szKBPath, "KeyMagic");
	CreateDirectory(szKBPath, NULL);
	PathAppend(szKBPath, "Keyboards");
	CreateDirectory(szKBPath, NULL);
next:
	LoadString(hInst, IDS_EN_TITLE, szTitle, MAX_LOADSTRING);
	SetWindowText(hWnd, szTitle);

	GetKeyBoards();

	if (hKeyMenu){
		NOTIFYICONDATA nid;
		nid.cbSize = sizeof(NOTIFYICONDATA);
		nid.hWnd = hWnd;
		nid.uID = TRAY_ID;
		nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		nid.uCallbackMessage = WM_TRAY;
		nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_KEYMAGIC));
		lstrcpy(nid.szTip, "KeyMagic");
		Shell_NotifyIcon(NIM_ADD,&nid);
		UpdateWindow(hWnd);
	}

	SetHook(hWnd);

	SendMessage(hWnd,WM_SETICON, 1,(LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_KEYMAGIC)));

	hMenu = GetSystemMenu(hWnd, FALSE);

	AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
	AppendMenu(hMenu, MF_BYCOMMAND, IDM_ABOUT, "&About");
	EnableWindow(GetDlgItem(hWnd,IDC_PATH), false);
	hList = GetDlgItem(hWnd, IDC_KEYBOARDS);

	UpdateDlgData(hWnd);
	
}

bool UpdateDlgData(HWND hWnd){
	KeyFileData *Data;
	char szKBPath[MAX_PATH], szKBNames[500], szCurDir[MAX_PATH], szMenuDisplay[MAX_PATH], szShortCut[10];
	HWND hDisplay,hHotKey;

	hDisplay = GetDlgItem(hWnd, IDC_DISPLAY);
	hHotKey = GetDlgItem(hWnd, IDC_SHORTCUT);
	
	if (FAILED(SHGetFolderPath(NULL,
		CSIDL_COMMON_APPDATA,
		NULL,
		SHGFP_TYPE_CURRENT,
		szKBPath))){
			return false;
	}

	PathAppend(szKBPath, "KeyMagic");
	
	GetModuleFileName(hInst, szCurDir, MAX_PATH);
	int i;
	for (i=lstrlen(szCurDir); szCurDir[i] != '\\'; i--){
	}
	szCurDir[i] = NULL;

	PathAppend(szCurDir, "KeyMagic.ini");

	GetPrivateProfileString(szKBP, NULL, NULL, (LPSTR)szKBNames, 500, szCurDir);

	for (int i=0, j=0,Length = lstrlen(&szKBNames[i]);
		Length > 0; 
		i+=Length+1, Length = lstrlen(&szKBNames[i]), j++){

			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)&szKBNames[i]);

			GetPrivateProfileString(szMS, &szKBNames[i], NULL, szMenuDisplay, 30, szCurDir);
			GetPrivateProfileString(szKBP, &szKBNames[i], NULL, szKBPath, MAX_PATH, szCurDir);
			WORD wHotkey = GetPrivateProfileInt(szSC, &szKBNames[i], 0, szCurDir);

			Data = (KeyFileData*)LocalAlloc(LPTR, sizeof(KeyFileData));
			Data->isNew = FALSE;
			lstrcpy(Data->Name,  &szKBNames[i]);
			lstrcpy(Data->Display, szMenuDisplay);
			lstrcpy(Data->Path, szKBPath);
			Data->wHotkey = wHotkey;
			SendMessage(hList, LB_SETITEMDATA, j, (LPARAM)Data);
	}
	return true;
}

void DeleteDlgData(HWND hWnd){
	KeyFileData *Data;
	int count = SendMessage(hList, LB_GETCOUNT, 0, 0);
	for (int i=0;count > i; i++){
		Data = (KeyFileData*)SendMessage(hList, LB_GETITEMDATA, i, NULL);
		LocalFree(Data);
	}
	SendMessage(hList, LB_RESETCONTENT, 0, 0);
}

void SetKbData(HWND hWnd){
	char szCurDir[MAX_PATH],shortcut[10], szNormal[] = "Normal";
	KeyFileData *Data;
	int count = SendMessage(hList, LB_GETCOUNT, 0, 0);

	GetModuleFileName(hInst, szCurDir, MAX_PATH);

	int i;
	for (i=lstrlen(szCurDir); szCurDir[i] != '\\'; i--){
	}

	szCurDir[i] = NULL;

	PathAppend(szCurDir, "KeyMagic.ini");

	WritePrivateProfileSection(szKBP, NULL, szCurDir);
	WritePrivateProfileSection(szMS, NULL, szCurDir);
	WritePrivateProfileSection(szSC, NULL, szCurDir);

	for (int i=0;count > i; i++){
		Data = (KeyFileData*)SendMessage(hList, LB_GETITEMDATA, i, NULL);

		WritePrivateProfileString(szKBP, Data->Name, Data->Path, szCurDir);

		WritePrivateProfileString(szMS, Data->Name, Data->Display, szCurDir);

		wsprintf(shortcut, "%d", Data->wHotkey);

		WritePrivateProfileString(szSC, Data->Name, shortcut, szCurDir);
	}
}

bool OpenDialog(HWND hwnd, char* szFileName,DWORD nMaxFile)
{
	OPENFILENAME ofn = {0};

	RtlZeroMemory(szFileName, nMaxFile);

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = hInst;
	ofn.lpstrFilter = "Keymap File(*.kmk)\0*.kmk\0\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = nMaxFile;
	ofn.lpstrTitle = "Open File...";
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | 
		OFN_LONGNAMES | OFN_HIDEREADONLY;

	if(!GetOpenFileName(&ofn)) return false;
	return true;
}

void restart (HWND hWnd){
	DeleteDlgData(hWnd);
	UpdateDlgData(hWnd);
	kbindex = -1;

	SetDlgItemText(hWnd, IDC_DISPLAY, NULL);
	SetDlgItemText(hWnd, IDC_PATH, NULL);
	SendDlgItemMessage(hWnd, IDC_SHORTCUT, HKM_SETHOTKEY ,0, 0);

	for (UINT uID = IDKM_NORMAL; uID < IDKM_ID+KeyBoardNum; uID++) 
	{
		DestroyMyMenu(hKeyMenu, uID);
	}

	SendMessage(hList, LB_SETSEL, 0, 0);

	GetKeyBoards();
}

bool AddKeyBoardToList(HWND hWnd,char* lpKBPath){
	char lpPath[MAX_PATH];
	char lpName[MAX_PATH];
	char szCurDir[MAX_PATH];
	char szKBPath[MAX_PATH];

	KeyFileData *Data;

	GetModuleFileName(hInst, szCurDir, MAX_PATH);

	int i;
	for (i=lstrlen(szCurDir); szCurDir[i] != '\\'; i--){
	}

	szCurDir[i] = NULL;
	PathAppend(szCurDir, "KeyMagic.ini");
	
	GetFileTitle(lpKBPath, lpName, MAX_PATH);
	wsprintf(lpPath, "KeyBoards\\%s", lpName);
	lpName [lstrlen(lpName)-4] = NULL;

	if (SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, szKBPath)){
		MessageBox(hWnd, "Cannot locate \"Common Application Data\" Path!", szKeymagic, MB_ICONERROR);
		return false;
	}

	PathAppend(szKBPath, "KeyMagic");
	PathAppend(szKBPath, lpPath);

	if (!CopyFile(lpKBPath, szKBPath, false)){
		if (IDNO == MessageBox(hWnd, "File copying fail! \n Do you want to use from current path?", szKeymagic, MB_ICONERROR | MB_YESNO))
			return false;
		lstrcpy(lpPath, lpKBPath);
	}

	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)lpName);
	Data = (KeyFileData*)LocalAlloc(LPTR, sizeof(KeyFileData));
	lstrcpy(Data->Name, lpName);
	lstrcpy(Data->Display, lpName);
	lstrcpy(Data->Path, lpPath);
	Data->wHotkey = 0;

	SendMessage(hList, LB_SETITEMDATA, SendMessage(hList, LB_GETCOUNT, 0, 0)-1, (LPARAM)Data);

	return true;
}

bool RemoveKeyBoard(){
	KeyFileData *Data;

	kbindex = SendMessage(hList, LB_GETCURSEL, 0, 0);

	if (kbindex == -1 || kbindex == 0)
		return false;

	Data = (KeyFileData*)SendMessage(hList, LB_GETITEMDATA, kbindex, 0);

	if (Data->Path[1] != ':'){
		if (cbFileToDelete == 0)
			szFileToDelete = (strDelete*)LocalAlloc(LPTR, 100);
		cbFileToDelete++;
		LocalReAlloc(szFileToDelete, sizeof (strDelete)*cbFileToDelete, LPTR);
		lstrcpy(szFileToDelete[cbFileToDelete].Path, Data->Path);
	}

	LocalFree(Data);
	SendMessage(hList, LB_DELETESTRING, kbindex, 0);
	kbindex=-1;
	SendMessage(hList, LB_SETCURSEL, 0, 0);
	return true;
}

bool DeleteKeyFile(){
	char szKBPath[MAX_PATH],szToDelete[MAX_PATH];

	if (SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, szKBPath)){
		cbFileToDelete=0;
		return false;
	}
	PathAppend(szKBPath, "KeyMagic");
	for (int i=0; i <= cbFileToDelete; i++){
		lstrcpy(szToDelete, szKBPath);
		PathAppend(szToDelete, szFileToDelete[i].Path);
		DeleteFile(szToDelete);
	}
	cbFileToDelete=0;
	return true;
}