//Dialog Procedures
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

#include "KeyMagic.h"
#include "MyMenu.h"
#include "MyButton.h"
#include "DlgProc.h"

strDelete *szFileToDelete;
int cbFileToDelete = 0;
int kbindex=-1;
HWND hList;
HWND LastHWND;
UINT KeyBoardNum;
DWORD StartupFlag;

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
		Up.Fill = RGB(240,250,255);
		Up.Frame = RGB(153,217,234);
		Up.Text = RGB(10,120,245);
		Down.Fill = RGB(210,240,255);
		Down.Frame = RGB(110,205,220);
		Down.Text = RGB(10,120,245);
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
			AppendMenu(hMenu, MF_BYCOMMAND, 101, "Manage &Keyboards");
			AppendMenu(hMenu, MF_BYCOMMAND, 102, "&Run at Startup");
			AppendMenu(hMenu, MF_BYCOMMAND, 103, "&About");
			AppendMenu(hMenu, MF_BYCOMMAND, 100, "E&xit");
			CreateMyMenu(hWnd, hMenu);

			StartupFlag = GetPrivateProfileInt("Settings", "Startup", 0, szINIFile);

			StartupFlag ? CheckMenuItem(hMenu, 102, MF_CHECKED) : CheckMenuItem(hMenu, 102, MF_UNCHECKED);

			GetCursorPos(&pt);

			SetForegroundWindow(hWnd);

			BOOL CMD= TrackPopupMenu(hMenu, TPM_RIGHTBUTTON | TPM_LEFTBUTTON | TPM_RETURNCMD,
				pt.x, pt.y, 0, hWnd, NULL);

			switch (CMD){
				case 100:
					PostQuitMessage(0);
					break;
				case 101:
					ShowWindow(hWnd, SW_SHOW);
					break;
				case 102:
					StartupFlag =! StartupFlag;
					Startup(StartupFlag);
					break;
				case 103:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
			}

			DestroyMyMenu(hMenu);

			DestroyMenu(hMenu);

			PostMessage(hWnd, WM_NULL, 0, 0);
		}
		break;
	case WM_CTLCOLORDLG:
		return (INT_PTR)CreateSolidBrush(RGB(236, 244, 255));
	//	break;

	case WM_CTLCOLORSTATIC:
		SetBkColor((HDC)wParam, RGB(236, 244, 255));
		return (INT_PTR)CreateSolidBrush(RGB(236, 244, 255));

	case WM_MEASUREITEM:
		OnMenuMeasure(hWnd, (LPMEASUREITEMSTRUCT)lParam);

		break;

	case WM_DRAWITEM:
		DrawMyMenu((LPDRAWITEMSTRUCT)lParam);
		DrawMyButton((LPDRAWITEMSTRUCT)lParam);

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

		DestroyMyMenu(hKeyMenu);

		DeleteDlgData(hWnd);

		EndDialog(hWnd,0);
		break;
	}
	return (INT_PTR)FALSE;
}

void OnInitDlg(HWND hWnd){
	HMENU hMenu;
	KeyFileData *Data;
	char szKBPath[MAX_PATH];

	LoadIcon(hInst, (LPCSTR)IDI_KEYMAGIC);

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
	
	if (hide == true)
		SetTimer(hWnd, 100, USER_TIMER_MINIMUM, NULL);

	CreateMyMenu(hWnd, hKeyMenu);
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
			error("SHGetFolderPath");
			return false;
	}

	PathAppend(szKBPath, "KeyMagic");

	GetPrivateProfileString(szKBP, NULL, NULL, (LPSTR)szKBNames, 500, szINIFile);

	for (int i=0, j=0,Length = lstrlen(&szKBNames[i]);
		Length > 0; 
		i+=Length+1, Length = lstrlen(&szKBNames[i]), j++){

			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)&szKBNames[i]);

			GetPrivateProfileString(szMS, &szKBNames[i], NULL, szMenuDisplay, 30, szINIFile);
			GetPrivateProfileString(szKBP, &szKBNames[i], NULL, szKBPath, MAX_PATH, szINIFile);
			WORD wHotkey = GetPrivateProfileInt(szSC, &szKBNames[i], 0, szINIFile);

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
	char shortcut[10], szNormal[] = "Normal";
	KeyFileData *Data;
	int count = SendMessage(hList, LB_GETCOUNT, 0, 0);

	WritePrivateProfileSection(szKBP, NULL, szINIFile);
	WritePrivateProfileSection(szMS, NULL, szINIFile);
	WritePrivateProfileSection(szSC, NULL, szINIFile);

	for (int i=0;count > i; i++){
		Data = (KeyFileData*)SendMessage(hList, LB_GETITEMDATA, i, NULL);

		WritePrivateProfileString(szKBP, Data->Name, Data->Path, szINIFile);

		WritePrivateProfileString(szMS, Data->Name, Data->Display, szINIFile);

		wsprintf(shortcut, "%d", Data->wHotkey);

		WritePrivateProfileString(szSC, Data->Name, shortcut, szINIFile);
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

	DestroyMyMenu(hKeyMenu);

	SendMessage(hList, LB_SETSEL, 0, 0);

	GetKeyBoards();
	CreateMyMenu(hWnd, hKeyMenu);
}

bool AddKeyBoardToList(HWND hWnd,char* lpKBPath){
	char lpPath[MAX_PATH];
	char lpName[MAX_PATH];
	char szKBPath[MAX_PATH];

	KeyFileData *Data;
	
	GetFileTitle(lpKBPath, lpName, MAX_PATH);

	if (lpName [lstrlen(lpName)-4] != '.')
		lstrcat(lpName, ".kmk");

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
		if (cbFileToDelete == 0){
			szFileToDelete = (strDelete*)VirtualAlloc(NULL, sizeof(strDelete)*20, MEM_COMMIT, PAGE_READWRITE);
		}

		cbFileToDelete++;
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

	if (!cbFileToDelete)
		return false;

	if (SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, szKBPath)){
		cbFileToDelete=0;
		error("SHGetFolderPath");
		return false;
	}

	PathAppend(szKBPath, "KeyMagic");
	for (int i=1; i <= cbFileToDelete; i++){
		lstrcpy(szToDelete, szKBPath);
		PathAppend(szToDelete, szFileToDelete[i].Path);
		DeleteFile(szToDelete);
	}

	VirtualFree(szFileToDelete, sizeof(strDelete)*20, MEM_DECOMMIT);

	cbFileToDelete=0;
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

void error(LPCSTR lpszFunction) 
{
	int *Buffer;
	//Get Last Error Code and Translate to text
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | 
		FORMAT_MESSAGE_FROM_SYSTEM, NULL, 
			GetLastError(),LANG_NEUTRAL, (LPSTR)&Buffer, 0, NULL);
	
	//Show the error message box
	MessageBoxA(NULL, (LPCSTR)Buffer, lpszFunction, MB_OK | MB_ICONHAND | MB_APPLMODAL);
}

void GetKeyBoards(){

	char szMenuDisplay[MAX_PATH];
	char szKBNames[500];
	char szShortCut[20];
	WORD wHotKey;

	if (hKeyMenu)
		DestroyMenu(hKeyMenu);

	hKeyMenu = CreatePopupMenu();
	if (!hKeyMenu)
		return;

	GetPrivateProfileString(szKBP, NULL, NULL, (LPSTR)szKBNames, 500, szINIFile);

	GetPrivateProfileString(szMS, &szKBNames[0], NULL, (LPSTR)szMenuDisplay, MAX_PATH, szINIFile);
	wHotKey = GetPrivateProfileInt(szSC, &szKBNames[0], 0, szINIFile);
	lstrcat(szMenuDisplay, "\t");

	GetHotKey(wHotKey, szShortCut);
	lstrcat(szMenuDisplay, szShortCut);

	AppendMenu(hKeyMenu, NULL, IDKM_NORMAL, szMenuDisplay);
	KeyBoardNum=0;
	for (int i=lstrlen(&szKBNames[0])+1,Length = lstrlen(&szKBNames[i]);
		Length > 0; 
		i+=Length+1, Length = lstrlen(&szKBNames[i])){
			GetPrivateProfileString(szMS, &szKBNames[i], NULL, szMenuDisplay, MAX_PATH, szINIFile);
			wHotKey = GetPrivateProfileInt(szSC, &szKBNames[i], 0, szINIFile);

			lstrcat(szMenuDisplay, "\t");
			GetHotKey(wHotKey, szShortCut);
			lstrcat(szMenuDisplay, szShortCut);

			AppendMenu(hKeyMenu, NULL, IDKM_ID+KeyBoardNum, szMenuDisplay);
			KeyBoardNum++;
	}

	CheckMenuRadioItem(hKeyMenu, IDKM_NORMAL, 
		KeyBoardNum + IDKM_ID,
		IDKM_NORMAL,
		MF_BYCOMMAND);

};

void Startup(BOOL isEnable){
	HKEY hkHLM, hkRun;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, NULL, NULL, KEY_ALL_ACCESS, &hkHLM) != ERROR_SUCCESS)
		return;
	if (RegOpenKeyEx(hkHLM, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", NULL, KEY_ALL_ACCESS, &hkRun) != ERROR_SUCCESS)
		return;
	if (!isEnable){
		RegDeleteValue(hkRun, "Keymagic");
	}
	else{
		char FileName[MAX_PATH];
		GetModuleFileName(hInst, FileName, MAX_PATH);
		lstrcat(FileName, " -s");
		RegSetValueEx(hkRun, "Keymagic", NULL, REG_SZ, (BYTE*)FileName, lstrlen(FileName));
	}

	isEnable ? WritePrivateProfileString("Settings", "Startup", "1", szINIFile) : WritePrivateProfileString("Settings", "Startup", "0", szINIFile);

	RegCloseKey(hkRun);
	RegCloseKey(hkHLM);
}