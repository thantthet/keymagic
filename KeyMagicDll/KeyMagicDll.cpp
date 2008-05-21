//The head of Keymagic application.
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
#include "KeyMagicDll.h"
#include "regex.h"

#pragma data_seg(".magic")
HHOOK hKeyHook = NULL;
HHOOK hMouseHook = NULL;
HHOOK hWndProcHook = NULL;
HHOOK hGetMsgHook = NULL;
HWND Commander_hWnd = NULL;
UINT KM_SETKBID = NULL;
UINT KM_KILLFOCUS = NULL;
UINT KM_CHAR = NULL;
UINT KM_KEYEVENT = NULL;
char szDir[MAX_PATH] = {0};
#pragma data_seg()

//Make sure that section can READ WRITE and SHARE
#pragma comment(linker, "/SECTION:.magic,RWS")

WPARAM	wPam = PM_REMOVE;
HANDLE	hFile;
LPVOID	FilePtr;
HANDLE	hFileMap;

KbFileHeader	*FileHeader = NULL;
KbData	data;
OrdData	*OData;
CombineData	*comdat;

int	LastIndex = -2;
wchar_t keystroke[5];
wchar_t	LatestSent [5];
wchar_t	LastInput [2];
bool	LastIsCombine;
bool	isActive = false;
bool	StillInHook;
bool	isInterBack = false;
UINT	CurOderIndx = 0;
UINT	LastCombineLen;
UINT	PostedKey = 0;
UINT	NumOfShortCut = 0;
UINT	ActiveIndex = -1;

bool	OpenForMapping(LPCSTR FileName);
bool	CheckForCombinationKeys(wchar_t uVKey);
bool	KeyProc(wchar_t wchar);
void	BackForCombination();
void	UpperOrLower (UINT *uVKey);
wchar_t	SendStrokes (wchar_t* Strokes);
LPCSTR	GetKeyBoard(UINT Index);
int	ShortCutCheck (UINT uvKey);

KM_ShortCut *SC;

int ShortCutCheck (UINT uvKey){

	int i;
	for (i=0; i < NumOfShortCut; i++){
		if (uvKey != SC[i].SC_KEY)
			continue;

		if (SC[i].SC_CTRL)
			if (!(GetKeyState(VK_CONTROL) & 0x8000))
				continue;

		if (SC[i].SC_ALT)
			if (!(GetKeyState(VK_MENU) & 0x8000))
				continue;

		if (SC[i].SC_SHIFT)
			if (!(GetKeyState(VK_SHIFT) & 0x8000))
				continue;
		break;
	}

	if (i == NumOfShortCut)
		return -1;

	return i;

}

LRESULT KEYMAGICDLL_API CALLBACK HookKeyProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	if (nCode == HC_ACTION && lParam & 0x80000000){
		return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
	}

	else if (nCode == HC_ACTION && lParam){

		int index = ShortCutCheck(wParam);
		if (index >= 0){
			if (ActiveIndex == index && isActive){
				isActive = false;
			}
			else
				OpenKbFile(index);
			return 1;
		}

		if (isActive == false)
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);

		if (!GetFocus())
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);

		if (GetKeyState(VK_MENU) & 0x8000 || GetKeyState(VK_CONTROL) & 0x8000)
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);

		if (wParam == VK_BACK){
			
			if (LastIsCombine){
				BackForCombination();
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

		uVKey = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);

		if (!uVKey)
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);

		UpperOrLower(&uVKey);

		wchar_t* pdest = wcsstr(data.vk, (wchar_t*)&uVKey);

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

wchar_t SendStrokes (wchar_t* Strokes)//Send Keys
{
	wchar_t Temp;

	Temp = *(Strokes+1);
	*(Strokes+1) = *Strokes;
	*Strokes = Temp;
	
	HWND hwnd = GetFocus();
	if (!LastIsCombine)
		LastCombineLen = 1;

	BackForCombination();

	PostMessageW(hwnd, KM_CHAR, (WPARAM)*Strokes, 0x210001);
	keystroke[0] = *Strokes;
	PostMessageW(hwnd, KM_CHAR, (WPARAM)*(Strokes+1), 0x210001);
	keystroke[1] = *(Strokes+1);
	
	LastIsCombine = FALSE;
	PostedKey = 0;

	return *(Strokes+1);
}

bool CheckForCombinationKeys(wchar_t uVKey)//Check for combination (eg. HA HTOE YA PIN [VK = Q] in MM)
{
	if (!FileHeader->nComData){
		return false;
	}
	
	for (int i=0;FileHeader->nComData > i; i++){
		if (comdat[i].Key == uVKey)
		{
			HWND handle = GetFocus();
			if (!handle)
				return false;
			int j = 0;
			do {
				PostMessageW(handle, KM_CHAR, (WPARAM)comdat[i].Data[j], 0x110001 );
				keystroke[j] = (WPARAM)comdat[i].Data[j];
				j++;
			}while (comdat[i].Data[j]);
			LastCombineLen = j;
			LastInput[0] = comdat[i].Data[LastCombineLen-1];
			PostedKey = NULL;
			return true;
		}
	}
	return false;
}

void BackForCombination(){//Just deleteing series of chars(Combination Chars)
	HWND handle = GetFocus();
	if (!handle)
		return;

	for (LastCombineLen; LastCombineLen > 0; LastCombineLen--)
		PostMessage(handle, KM_KEYEVENT, VK_BACK, NULL);
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
			GetClassName(cwp->hwnd, (LPSTR)Name, 50);
			if (!lstrcmp(Name, "KEYMAGIC"))
				break;
			if (!lstrcmp(Name,"Shell_TrayWnd"))
				break;
			if (!lstrcmp(Name, "TrayNotifyWnd"))
				break;

			PostMessage(HWND_BROADCAST, KM_KILLFOCUS, 0,(LPARAM) cwp->hwnd);
		}
		break;
	//case WM_SETFOCUS:
	//	break;
	}

	if (cwp->message == KM_SETKBID){
		isActive = cwp->lParam;
		if (isActive)
			OpenKbFile(cwp->wParam);
	}

	return CallNextHookEx(hWndProcHook, nCode, wParam, lParam);
}


LRESULT KEYMAGICDLL_API CALLBACK HookGetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return CallNextHookEx(hGetMsgHook, nCode, wParam, lParam);

	if (wParam == PM_NOREMOVE && wPam == PM_REMOVE){
		wPam = PM_NOREMOVE;
	}

	MSG* msg = (MSG*)lParam;
	if ((msg->message == WM_KEYDOWN || msg->message == WM_KEYUP) &&
		(msg->wParam == VK_LEFT || msg->wParam == VK_RIGHT)){
			LastIsCombine = FALSE;
			LastInput[0] = NULL;
			LatestSent[0] = NULL;
	}

	if (msg->message == KM_CHAR && wParam == wPam){
		//PostMessageW(msg->hwnd, WM_CHAR, msg->wParam, msg->lParam );
		if (msg->wParam != keystroke[PostedKey])
			return CallNextHookEx(hGetMsgHook, nCode, wParam, lParam);
		INPUT ip;
		KEYBDINPUT ki;
		ki.dwExtraInfo = 0;
		ki.dwFlags = KEYEVENTF_UNICODE;
		ki.wScan = msg->wParam;
		ki.wVk = 0;
		ki.time = 0;

		ip.type = INPUT_KEYBOARD;
		ip.ki = ki;

		SendInput(1, &ip, sizeof(INPUT));
		LatestSent[0] = msg->wParam;
		PostedKey++;
		return 1;
	}

	if (msg->message == KM_KEYEVENT && wParam == wPam){
		keybd_event(255, 2, KEYEVENTF_KEYUP, NULL);
		keybd_event(msg->wParam, 255, NULL, NULL);
		keybd_event(msg->wParam, 2, KEYEVENTF_KEYUP, NULL);
	}
	
	return CallNextHookEx(hGetMsgHook, nCode, wParam, lParam);
}

bool KeyProc(wchar_t wchar){

	for (int i=0;FileHeader->nOrdData > i; i++){
		LastInput[1] = 0;
		wchar_t Dest[2];
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
		bool isMatch = or.test((wchar_t*)Dest);

		if (isMatch){
			SendStrokes((wchar_t*)Dest);
			if (!LatestSent[0]){
				return false;
			}
			return true;
		}
	}
	
	isInterBack = LastIsCombine = FALSE;
	PostMessageW(GetFocus(), KM_CHAR, wchar, 0x110001 );
	keystroke[0] =  LastInput[0] = wchar;
	PostedKey = 0;
	return true;
}

void KEYMAGICDLL_API HookInit(HWND hWnd,HHOOK hKbHook,HHOOK hWPHook, HHOOK hGMHook, 
							  UINT KM_SETKB_ID,  UINT KM_KILL_FOCUS, LPCSTR ParentPath)
{
	Commander_hWnd = hWnd;
	hKeyHook = hKbHook;
	hWndProcHook = hWPHook;
	hGetMsgHook = hGMHook;
	KM_SETKBID = KM_SETKB_ID;
	KM_KILLFOCUS = KM_KILL_FOCUS;
	lstrcpy(szDir, ParentPath);

	KM_CHAR = RegisterWindowMessage("KM_CHAR");
	KM_KEYEVENT = RegisterWindowMessage("KM_KEYEVENT");
}

bool OpenKbFile(int Index)
{
	char* Msg[300];

	if (!OpenForMapping(GetKeyBoard(Index))){
		wsprintf((LPSTR)Msg, "Cannot open \"%s\"!", GetKeyBoard(Index));
		MessageBox(Commander_hWnd, (LPSTR)Msg , "KeyMagic", MB_ICONEXCLAMATION | MB_OK);
		isActive = false;
		return false;
	}
	FileHeader = (KbFileHeader*)FilePtr;
	if (FileHeader->Magic != 'FKMK'){
		wsprintf((LPSTR)Msg, "\"%s\"\n is not a vaild KeyMagic's keyboard file!", GetKeyBoard(Index));
		MessageBox(Commander_hWnd, (LPSTR)Msg , "KeyMagic", MB_ICONEXCLAMATION | MB_OK);
		isActive = false;
		return false;
	}

	data.vk = (wchar_t*)FilePtr+6;
	data.uc = (wchar_t*)FilePtr+6+(WORD)FileHeader->lvk;
	OData = (OrdData*)(FileHeader->luc+ data.uc);
	comdat = (CombineData*)((char*)OData+sizeof(OrdData)*FileHeader->nOrdData);

	ActiveIndex = Index;
	isActive = true;
	return true;
}

bool OpenForMapping(LPCSTR FileName)
{
	CloseMapping();

	hFile = hFileMap = FilePtr = NULL;

	hFile = CreateFileA(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
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
	char szINI[500];
	char szKBNames[500];
	char szKBFile[200];
	char szKBP[] ="KeyBoardPaths";

	lstrcpy(szINI,szDir);
	lstrcat(szINI,"\\KeyMagic.ini");

	GetPrivateProfileString(szKBP, NULL, NULL, (LPSTR)szKBNames, 500, szINI);

	for (int i=0,Length = lstrlen(&szKBNames[i]),j=0; 
		j <= Index;
		i+=Length+1, j++, Length = lstrlen(&szKBNames[i])){
			GetPrivateProfileString(szKBP, (LPCSTR)&szKBNames[i], NULL, (LPSTR)szKBFile, 500, szINI);
	}

	lstrcpy(szINI, szDir);
	lstrcat(szINI, szKBFile);
	return szINI;
};

void GetShortCuts(){
	char szINI[500];
	char szKBNames[500];
	char szShortCut[50];
	char szKBP[] ="KeyBoardPaths";
	char szSC[] ="ShortCuts";

	lstrcpy(szINI,szDir);
	lstrcat(szINI,"\\KeyMagic.ini");

	GetPrivateProfileString(szKBP, NULL, NULL, (LPSTR)szKBNames, 500, szINI);

	SC = (KM_ShortCut*)malloc(sizeof(KM_ShortCut)*100);

	for (int i=0,Length = lstrlen(&szKBNames[i]);
		Length > 0;
		i+=Length+1, Length = lstrlen(&szKBNames[i])){
			if (GetPrivateProfileString(szSC, (LPCSTR)&szKBNames[i], NULL, (LPSTR)szShortCut, 50, szINI)){
				sscanf(szShortCut, "%d+%d+%d+%s", &SC[i].SC_CTRL, &SC[i].SC_ALT, &SC[i].SC_SHIFT, &SC[i].SC_KEY);
				NumOfShortCut++;
			}
	}

}

void UpperOrLower (UINT *uVKey){

	bool shiftDown = GetKeyState(VK_SHIFT) & 0x8000;
	bool capsToggled = GetKeyState(VK_CAPITAL) & 0x1;
	bool isUp = false;

	if ( (!shiftDown && capsToggled) || (shiftDown && !capsToggled) )
	{
		isUp = true;
	}

	if (  ( ( (*uVKey >= '0') && (*uVKey <= '9') ) || 
		(*uVKey == '`') || (*uVKey == '-') || (*uVKey == '=') || (*uVKey == '[') || (*uVKey == ']') || 
		(*uVKey == '\\') || (*uVKey == ';') || (*uVKey == '\'') || (*uVKey == ',') || (*uVKey == '.') || 
		(*uVKey == '/')) 
		&& isUp ){
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
}