//The head of Keymagic application.
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
#include <stdio.h>

#include "KeyMagicDll.h"
#include "regex.h"

#pragma data_seg(".keymagic")
HHOOK hKeyHook = NULL;
HHOOK hWndProcHook = NULL;
HHOOK hGetMsgHook = NULL;
HWND Commander_hWnd = NULL;
char szDir[1000] = {0};
#pragma data_seg()

//Make sure that section can READ WRITE and SHARE
#pragma comment(linker, "/SECTION:.keymagic,RWS")

//WPARAM	wPam = PM_REMOVE;
HANDLE	hFile;
LPVOID	FilePtr;
HANDLE	hFileMap;

KbFileHeader *FileHeader;
KbData	data;
OrdData	*OData;
CombineData	*comdat;
UniqueKey *UKey;
KM_ShortCut *SC;

//wchar_t keystroke[5];
wchar_t	LatestSent [5];
wchar_t	LastInput [2];
bool	LastIsCombine;
bool	isActive = false;
bool	isInterBack = false;
UINT	CurOderIndx = 0;
UINT	LastCombineLen;
//UINT	PostedKey = 0;
UINT	NumOfShortCut = 0;
UINT	ActiveIndex = 0;

bool	OpenForMapping(LPCSTR FileName);
bool	CheckForCombinationKeys(wchar_t uVKey);
bool	KeyProc(wchar_t wchar);
bool	UniqueProc(char uVKey);
void	MyBack();
void	UpperOrLower (UINT *uVKey);
wchar_t	SendStrokes (wchar_t* Strokes);
LPCSTR	GetKeyBoard(UINT Index);
int		ShortCutCheck (UINT uvKey);
void	Logger(char* fmt, ...);

//#define DEBUG 1

//Will active in later version
struct POSITION{
	UINT LenToBack;
	wchar_t CharToRestore;
};

POSITION Recent;

int ShortCutCheck (UINT uvKey){
	Logger("ShortCutCheck Entry");
	bool isCTRL, isALT, isSHIFT;

	BYTE KeyStatus[256];
	GetKeyboardState(KeyStatus);

	isCTRL = KeyStatus[VK_CONTROL] & 0x80;
	isALT = KeyStatus[VK_MENU] & 0x80;
	isSHIFT = KeyStatus[VK_SHIFT] & 0x80;

	Logger("ShortCutCheck StartChecking");
	for (int i=0; i < NumOfShortCut; i++){
		Logger("ShortCutCheck : Not Found Yet i = %X",i);
		if (uvKey != SC[i].ukey){
			continue;
		}

		if (SC[i].modkey & HOTKEYF_SHIFT){
			if (isSHIFT == false)
				continue;
		}

		if (SC[i].modkey & HOTKEYF_CONTROL){
			if (isCTRL == false)
				continue;
		}

		if (SC[i].modkey & HOTKEYF_ALT){
			if (isALT == false)
				continue;
		}

		return i;
	}
	Logger("ShortCutCheck : return -1");

	return -1;
}

LRESULT KEYMAGICDLL_API CALLBACK HookKeyProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	if (nCode < 0){
		return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
	}

	if (nCode == HC_ACTION && lParam & 0x80000000){
		return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
	}

	else if (nCode == HC_ACTION && lParam){

		if (!GetFocus())
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);

		int index = ShortCutCheck(wParam);
		if (index >= 0){
			if ( (ActiveIndex == index && isActive) || index == 0){
				isActive = false;
				PostMessage(Commander_hWnd, KM_GETFOCUS, 0, 0);
			}
			else{
				OpenKbFile(index);
				PostMessage(Commander_hWnd, KM_GETFOCUS, index, 0);
			}
			return 1;
		}

		if (isActive == false)
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);

		if (UniqueProc(wParam))
			return 1;

		Logger("Checking MenuKey and ControlKey");
		if (GetKeyState(VK_MENU) & 0x8000 || GetKeyState(VK_CONTROL) & 0x8000)
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);

		if (wParam == VK_BACK){
			
			if (LastIsCombine){
				MyBack();
				return 1;
			}
			LastInput[0] = NULL;
			if (!isInterBack)
				LatestSent[0] = NULL;
		}

		if (wParam == VK_RIGHT || wParam == VK_LEFT){
			LastIsCombine = FALSE;
		}

		UINT uVKey;
		Logger("Mapping Virtual Key : uVKey = %X",wParam);
		uVKey = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);

		if (!uVKey)
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);

		UpperOrLower(&uVKey);

		Logger("Processing wcsstr uVKey = %X",uVKey);
		wchar_t* pdest = wcsstr(data.vk, (wchar_t*)&uVKey);
		Logger("wcsstr done");

		Logger("pdest = %X", pdest);
		if (pdest){
			int result = (int)(pdest - data.vk);
			if (!GetFocus())
				return CallNextHookEx(hKeyHook, nCode, wParam, lParam);

			if (KeyProc(data.uc[result]))
				return 1;
		}
		
		else if(CheckForCombinationKeys(uVKey))
		{
			LastIsCombine = TRUE;
			return 1;
		}
	}

	return CallNextHookEx(hKeyHook, nCode, wParam, lParam);

}

wchar_t SendStrokes (wchar_t* Strokes)//Send Keys Strokes
{
	Logger("SendStrokes : FirstStroke %X",Strokes[1]);
	HWND hwnd = GetFocus();

	int i;
	int len = wcslen(Strokes);

	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.dwExtraInfo = 0;
	ip.ki.dwFlags = KEYEVENTF_UNICODE;
	ip.ki.wVk = 0;
	ip.ki.time = 0;

	for(i=0; i < len; i++){
		//PostMessageW(hwnd, KM_CHAR, (WPARAM)Strokes[i], 0x210001);
		ip.ki.wScan = Strokes[i];

		SendInput(1, &ip, sizeof(INPUT));
	}
	LatestSent[0] = Strokes[i-1];

	//i--;
	//if (i > 0){
	//	keystroke[0] = Strokes[i-1];
	//	keystroke[1] = Strokes[i];
	//}

	//else{
	//	keystroke[0] = Strokes[i];
	//	keystroke[1] = 0;
	//}

	LastIsCombine = FALSE;
	//PostedKey = 0;

	Logger("SendStrokes : Return %X",Strokes[i]);
	return Strokes[i];
}

bool CheckForCombinationKeys(wchar_t uVKey)//Check for combination (eg. HA HTOE YA PIN [VK = Q] in MM)
{
	Logger("CheckForCombinationKeys : uVKey %X",uVKey);
	if (!FileHeader->nComData){
		return false;
	}
	
	for (int i=0;FileHeader->nComData > i; i++){
		if (comdat[i].Key == uVKey)
		{
			HWND handle = GetFocus();
			if (!handle){
				Logger("CheckForCombinationKeys : Return false");
				return false;
			}
			int j = 0;
			do {
				
				INPUT ip;

				ip.type = INPUT_KEYBOARD;
				ip.ki.dwExtraInfo = 0;
				ip.ki.dwFlags = KEYEVENTF_UNICODE;
				ip.ki.wScan = comdat[i].Data[j];
				ip.ki.wVk = 0;
				ip.ki.time = 0;

				SendInput(1, &ip, sizeof(INPUT));

				j++;
			}while (comdat[i].Data[j]);
			LastCombineLen = j;
			LatestSent[0] = LastInput[0] = comdat[i].Data[LastCombineLen-1];
			Logger("CheckForCombinationKeys : Return true");
			return true;
		}
	}
	Logger("CheckForCombinationKeys : Return false");
	return false;
}

void MyBack(){//Just deleteing series of chars(Combination Chars)
	HWND handle = GetFocus();
	if (!handle)
		return;

	for (LastCombineLen; LastCombineLen > 0; LastCombineLen--)
	{
		//PostMessage(handle, KM_KEYEVENT, VK_BACK, NULL);
		keybd_event(VK_BACK, 255, NULL, NULL);
		keybd_event(VK_BACK, 2, KEYEVENTF_KEYUP, NULL);
	}
	LastCombineLen = 0;
	isInterBack = true;

	LastIsCombine = FALSE;
}

LRESULT KEYMAGICDLL_API CALLBACK HookWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	char Name[50];

	if (nCode < 0)
		return CallNextHookEx(hWndProcHook, nCode, wParam, lParam);

	CWPSTRUCT* cwp = (CWPSTRUCT*)lParam;
	
	switch (cwp->message)
	{
	case WM_ACTIVATE:
		if (LOWORD(cwp->wParam) == WA_INACTIVE){

			if (cwp->hwnd == Commander_hWnd)
				break;

			GetClassName(cwp->hwnd, (LPSTR)Name, 50);

			if (!lstrcmp(Name,"Shell_TrayWnd"))
				break;
			if (!lstrcmp(Name, "TrayNotifyWnd"))
				break;

			PostMessage(Commander_hWnd, KM_KILLFOCUS, 0,(LPARAM) cwp->hwnd);
		}
		if (LOWORD(cwp->wParam) == WA_ACTIVE){

			if (cwp->hwnd == Commander_hWnd)
				break;

			GetClassName(cwp->hwnd, (LPSTR)Name, 50);
			
			if (!lstrcmp(Name,"Shell_TrayWnd"))
				break;
			if (!lstrcmp(Name, "TrayNotifyWnd"))
				break;

			if (isActive)
				PostMessage(Commander_hWnd, KM_GETFOCUS, ActiveIndex,(LPARAM) cwp->hwnd);
			else if (!isActive)
				PostMessage(Commander_hWnd, KM_GETFOCUS, 0, (LPARAM) cwp->hwnd);
		}
		break;

	case KM_SETKBID:
		isActive = cwp->lParam;
		if (isActive){
			if (cwp->wParam == ActiveIndex){
				isActive = false;
				ActiveIndex = -1;
			}
			else{
				OpenKbFile(cwp->wParam);
				ActiveIndex = cwp->wParam;
			}
		}
		break;
	case KM_RESCAN:
		LocalFree(SC);
		GetShortCuts();
	}

	return CallNextHookEx(hWndProcHook, nCode, wParam, lParam);
}


LRESULT KEYMAGICDLL_API CALLBACK HookGetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	if (nCode < 0)
		return CallNextHookEx(hGetMsgHook, nCode, wParam, lParam);

	MSG* msg = (MSG*)lParam;
	switch (msg->message){
		case WM_KEYDOWN:
		case WM_KEYUP:
			if (msg->wParam == VK_LEFT || msg->wParam == VK_RIGHT){
				LastIsCombine = FALSE;
				LastInput[0] = NULL;
				LatestSent[0] = NULL;
			}
			break;
	}

	return CallNextHookEx(hGetMsgHook, nCode, wParam, lParam);
}
bool UniqueProc(char uVKey){
	Logger("UniqueProc : uVKey %X",uVKey);
	bool isCTRL, isLALT, isRALT, isSHIFT;

	//isCTRL = GetKeyState(VK_CONTROL) & 0x8000;
	//isLALT = GetKeyState(VK_LMENU) & 0x8000;
	//isRALT = GetKeyState(VK_RMENU) & 0x8000;
	//isSHIFT = GetKeyState(VK_SHIFT) & 0x8000;

	BYTE KeyState[256];
	Logger("Processing GetKeyboardState");
	GetKeyboardState(KeyState);

	isCTRL = KeyState[VK_CONTROL] & 0x80;
	isLALT = KeyState[VK_LMENU] & 0x80;
	isRALT = KeyState[VK_RMENU] & 0x80;
	isSHIFT = KeyState[VK_SHIFT] & 0x80;

	Logger("UniqueProc : Loop Starting -> uVKey = %X, CTRL = %X, LALT = %X, RALT = %X, SHIFT = %X,", uVKey, KeyState[VK_CONTROL], KeyState[VK_LMENU], KeyState[VK_RMENU], KeyState[VK_SHIFT]);
	for (int i=0; FileHeader->nUnKey > i; i++){
		Logger("UniqueProc : Not found Yet i=%X",i);
		if (UKey[i].vKEY != uVKey){
			continue;
		}

		if (UKey[i].CTRL == isCTRL && UKey[i].R_ALT == isRALT && UKey[i].L_ALT == isLALT && UKey[i].SHIFT == isSHIFT)
		{
			Logger("UniqueProc : found uVKey = %X",UKey[i].wChars);
			SendStrokes(UKey[i].wChars);
			LastIsCombine = TRUE;
			LastCombineLen = wcslen(UKey[i].wChars);
			return true;
		}

	}

	Logger("UniqueProc : return false");
	return false;
}

bool KeyProc(wchar_t wchar){
	Logger("KeyProc Entry");
	for (int i=0;FileHeader->nOrdData > i; i++){
		LastInput[1] = 0;
		wchar_t Dest[5]={0};
		Regex CheckKey((wchar_t*)OData[i].Key);
		if (OData[i].Method == 'k'){
			Dest[0] = LastInput[0];
			if (!CheckKey.test(&LastInput[0]))
			{
				continue;
			}
		}
		else if (OData[i].Method == 'e'){
			Dest[0] = LatestSent[0];
			if (!CheckKey.test(&LatestSent[0]))
			{
				continue;
			}
		}

		Regex or((wchar_t*)OData[i].Data);

		Dest[1] = wchar;
		Logger("KeyProc : Matching Regex");
		bool isMatch = or.test((wchar_t*)Dest);

		if (isMatch){
			Logger("KeyProc : isMatch true");
			wchar_t Temp;

			Temp = *(Dest+1);
			*(Dest+1) = *Dest;
			*Dest = Temp;
			if (!LastIsCombine)
				LastCombineLen = 1;
			MyBack();
			SendStrokes((wchar_t*)Dest);
			return true;
		}
	}
	
	isInterBack = LastIsCombine = FALSE;
	Logger("KeyProc : Posting KM_CHAR wchar = %X",wchar);
	//PostMessageW(GetFocus(), KM_CHAR, wchar, 0x210001 );
	
	INPUT ip;

	ip.type = INPUT_KEYBOARD;
	ip.ki.dwExtraInfo = 0;
	ip.ki.dwFlags = KEYEVENTF_UNICODE;
	ip.ki.wScan = wchar;
	ip.ki.wVk = 0;
	ip.ki.time = 0;

	SendInput(1, &ip, sizeof(INPUT));


	LatestSent[0] = LastInput[0] = wchar;
	//keystroke[0] = LastInput[0] = wchar;
	//PostedKey = 0;
	return true;
}

void KEYMAGICDLL_API HookInit(HWND hWnd,HHOOK hKbHook,HHOOK hWPHook, HHOOK hGMHook,LPCSTR ParentPath)
{

	Commander_hWnd = hWnd;
	hKeyHook = hKbHook;
	hWndProcHook = hWPHook;
	hGetMsgHook = hGMHook;
	lstrcpy(szDir, ParentPath);

	//KM_CHAR = RegisterWindowMessage("KM_CHAR");
	//KM_KEYEVENT = RegisterWindowMessage("KM_KEYEVENT");

}

bool OpenKbFile(int Index)
{
	char Msg[300];

	if (!OpenForMapping(GetKeyBoard(Index))){
		wsprintf((LPSTR)Msg, "Cannot open \"%s\"!", GetKeyBoard(Index));
		if (lstrcmpi(&Msg[lstrlen(Msg)-3], "kmk"))
			return false;
		MessageBox(GetDesktopWindow(), (LPSTR)Msg , "KeyMagic", MB_ICONEXCLAMATION | MB_OK);
		isActive = false;
		return false;
	}

	FileHeader = (KbFileHeader*)FilePtr;

	if (FileHeader->Magic != 'FKMK'){
		wsprintf((LPSTR)Msg, "\"%s\"\n is not a vaild KeyMagic's keyboard file!", GetKeyBoard(Index));
		MessageBox(GetDesktopWindow(), (LPSTR)Msg , "KeyMagic", MB_ICONEXCLAMATION | MB_OK);
		isActive = false;
		return false;
	}

	data.vk = (wchar_t*)((PBYTE)FilePtr+sizeof(KbFileHeader));
	data.uc = (wchar_t*)((PBYTE)FilePtr+sizeof(KbFileHeader))+(WORD)FileHeader->lvk;
	OData = (OrdData*)(FileHeader->luc+ data.uc);
	comdat = (CombineData*)((PBYTE)OData+sizeof(OrdData)*FileHeader->nOrdData);
	UKey = (UniqueKey*)((PBYTE)comdat+sizeof(CombineData)*FileHeader->nComData);

	ActiveIndex = Index;
	isActive = true;
	return true;
}

bool OpenForMapping(LPCSTR FileName)
{
	CloseMapping();

	hFile = hFileMap = FilePtr = NULL;

	hFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}


	hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);

	if(hFileMap == NULL)
	{
		CloseHandle(hFile);
		hFile = NULL;
		return false;
	}

	FilePtr = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);

	if(FilePtr == NULL)
	{
		CloseHandle(hFileMap);
		CloseHandle(hFile);
		hFileMap = NULL;
		hFile = NULL;
		return false;
	}

	return true;
}

void CloseMapping()
{
	if(FilePtr != NULL)
	{
		UnmapViewOfFile(FilePtr);
		FilePtr = NULL;
	}
	if(hFileMap != NULL)
	{
		CloseHandle(hFileMap);
		hFileMap = NULL;
	}
	if(hFile != NULL)
	{
		CloseHandle(hFile);
		hFile = NULL;
	}
}

LPCSTR GetKeyBoard(UINT Index){
	char szINI[MAX_PATH];
	char szKBNames[500];
	char szKBFile[MAX_PATH];
	char szKBPath[MAX_PATH];
	char szAllUser[MAX_PATH];
	char szKBP[] ="KeyBoardPaths";

	lstrcpy(szINI,szDir);
	PathAppend(szINI, "KeyMagic.ini");

	GetPrivateProfileString(szKBP, NULL, NULL, (LPSTR)szKBNames, 500, szINI);

	for (int i=0,Length = lstrlen(&szKBNames[i]),j=0; 
		j <= Index;
		i+=Length+1, j++, Length = lstrlen(&szKBNames[i])){
			GetPrivateProfileString(szKBP, (LPCSTR)&szKBNames[i], NULL, (LPSTR)szKBFile, MAX_PATH, szINI);
	}

	if (szKBFile[1] == ':')
		return szKBFile;
	
	if (SUCCEEDED(SHGetFolderPath(NULL,
		CSIDL_COMMON_APPDATA,
		NULL,
		SHGFP_TYPE_CURRENT,
		szAllUser))){
		lstrcpy(szKBPath, szAllUser);
			PathAppend(szKBPath, "KeyMagic");
			PathAppend(szKBPath, szKBFile);
			return szKBPath;
	}
	return NULL;
};

void GetShortCuts(){
	Logger("GetShortCuts Entry");
	char szINI[MAX_PATH]={0};
	char szKBNames[MAX_PATH]={0};
	char szKBP[] ="KeyBoardPaths";
	char szSC[] ="ShortCuts";

	lstrcpy(szINI,szDir);
	lstrcat(szINI,"\\KeyMagic.ini");

	GetPrivateProfileString(szKBP, NULL, NULL, (LPSTR)szKBNames, 500, szINI);

	SC = (KM_ShortCut*) LocalAlloc(LPTR, sizeof(KM_ShortCut)*50);

	NumOfShortCut=0;
	for (int i=0,j=0,Length = lstrlen(&szKBNames[i]);
		Length > 0;
		i+=Length+1, Length = lstrlen(&szKBNames[i]),j++){
			WORD Hotkey = GetPrivateProfileInt(szSC, &szKBNames[i], 0, szINI);
			if (Hotkey){
				SC[j].ukey = (char) Hotkey;
				SC[j].modkey = Hotkey >> 8;
			}
			NumOfShortCut++;
	}

	LocalReAlloc(SC, sizeof(KM_ShortCut)*NumOfShortCut, LPTR);

	Logger("GetShortCuts Return : NumOfShortCut = %X ", NumOfShortCut);
}

void UpperOrLower (UINT *uVKey){
	Logger("UpperAndLower Entry");

	bool shiftDown;// = GetKeyState(VK_SHIFT) & 0x8000;
	bool capsToggled;// = GetKeyState(VK_CAPITAL) & 0x1;
	bool isUp = false;

	BYTE KeyStatus[256];
	GetKeyboardState(KeyStatus);

	shiftDown = KeyStatus[VK_SHIFT] & 0x80;
	capsToggled = KeyStatus[VK_CAPITAL] & 0x1;

	Logger("UpperAndLower : isUp = %X", isUp);

	if ( (!shiftDown && capsToggled) || (shiftDown && !capsToggled) )
	{
		isUp = true;
	}

	if ( isUp ){
		switch (*uVKey)
		{
		case '0':
			*uVKey = ')';
			break;
		case '1':
			*uVKey = '!';
			break;
		case '2':
			*uVKey = '@';
			break;
		case '3':
			*uVKey = '#';
			break;
		case '4':
			*uVKey = '$';
			break;
		case '5':
			*uVKey = '%';
			break;
		case '6':
			*uVKey = '^';
			break;
		case '7':
			*uVKey = '&';
			break;
		case '8':
			*uVKey = '*';
			break;
		case '9':
			*uVKey = '(';
			break;
		case '-':
			*uVKey = '_';
			break;
		case '=':
			*uVKey = '+';
			break;
		case '`':
			*uVKey = '~';
			break;
		case '[':
			*uVKey = '{';
			break;
		case ']':
			*uVKey = '}';
			break;
		case '\\':
			*uVKey = '|';
			break;
		case ';':
			*uVKey = ':';
			break;
		case '\'':
			*uVKey = '"';
			break;
		case ',':
			*uVKey = '<';
			break;
		case '.':
			*uVKey = '>';
			break;
		case '/':
			*uVKey = '?';
			break;
		}
	}

	else if ( (*uVKey >= 'A') && (*uVKey <= 'Z') &&!isUp)
		*uVKey += 32;

	Logger("UpperAndLower Return");
}

void Logger(char* fmt, ...)
{
#ifdef DEBUG
	char Memory[100];
	RECT rc;

	va_list list;

	va_start(list, fmt);
	//Format
	wvsprintf(Memory, fmt,list);

	GetClientRect(Commander_hWnd, &rc);

	rc.top += 50;
	SetWindowText(Commander_hWnd, Memory);
	
	//Cleanup
	va_end(list);
#else
	return;
#endif
}