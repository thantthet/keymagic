#include "KeyMagic.h"
#include "MyMenu.h"
#include "DlgProc.h"

strDelete *szFileToDelete;
int cbFileToDelete = 0;
int kbindex=-1;
HWND hList;
HANDLE hHeap;
void error(LPCSTR lpszFunction) ;

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
	
	if (!GetModuleFileName(hInst, szCurDir, MAX_PATH)){
		error("GetModuleFileName");
		return false;
	}

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
		if (cbFileToDelete == 0){
			//szFileToDelete = (strDelete*)LocalAlloc(LPTR, 100);
			hHeap = HeapCreate(0, 1000, 0);
			szFileToDelete = (strDelete*)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 100);
		}

		cbFileToDelete++;
		if (!HeapReAlloc(hHeap, HEAP_ZERO_MEMORY, szFileToDelete, sizeof (strDelete)*cbFileToDelete)){//LocalReAlloc(szFileToDelete, sizeof (strDelete)*cbFileToDelete, LPTR)){
			cbFileToDelete=0;
			HeapDestroy(hHeap);
			return false;
		}
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

	//LocalFree(szFileToDelete);
	if (hHeap){
		HeapFree(hHeap, HEAP_ZERO_MEMORY, szFileToDelete);
		HeapDestroy(hHeap);
	}

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