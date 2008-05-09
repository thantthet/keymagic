// KeyMagicDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "KeyMagicDll.h"
#include "Order.h"

#pragma data_seg(".magic")
HHOOK hKeyHook = NULL;
HHOOK hMouseHook = NULL;
HHOOK hWndProcHook = NULL;
HHOOK hGetMsgHook = NULL;
HWND Commander_hWnd = NULL;
UINT KM_SETKBID = NULL;
UINT KM_KILLFOCUS = NULL;
char szDir[MAX_PATH] = {0};
#pragma data_seg()

#pragma comment(linker, "/SECTION:.magic,RWS")

int		CheckToOrder(wchar_t KeyIn);

HANDLE	hFile;
LPVOID	FilePtr;
HANDLE	hFileMap;

KbFileHeader *FileHeader = NULL;
KbData	data;
OrdData	*OData;
CombineData *comdat;

UINT CurOderIndx = 0;
UINT KM_CHAR;

int Index = -1;
int LastIndex = -2;
wchar_t LatestSent [5];

UINT LastCombineLen;
bool LastIsCombine;
bool isActive = false;

bool	OpenForMapping(LPCSTR FileName);
int		WillPost;
bool	CheckForCombinationKeys(wchar_t uVKey);
void	BackForCombination();
LPCSTR	GetKeyBoard(UINT Index);
Order	or;
void UpperOrLower (UINT *uVKey);
bool StillInHook;
LRESULT KEYMAGICDLL_API CALLBACK HookKeyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	StillInHook = true;
	if (nCode == HC_ACTION && lParam & 0x80000000){
		return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
	}
	else if (nCode == HC_ACTION && lParam){
		if (isActive == false)
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
		if (!GetFocus())
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);

		if (GetKeyState(VK_MENU) & 0x8000 || GetKeyState(VK_CONTROL) & 0x8000)
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);

		if (wParam == VK_BACK){
			LatestSent[0] = NULL;
			if (LastIsCombine)
				BackForCombination();
		}

		if (wParam == VK_RIGHT || wParam == VK_LEFT)
			LastCombineLen = 0;

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

			WillPost = 1;
			if (!GetFocus())
				return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
			PostMessage(GetFocus(), KM_CHAR, (WPARAM)data.uc[result], 0x110001 );
			LastCombineLen = 0;
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

bool CheckForCombinationKeys(wchar_t uVKey)
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
				PostMessageW(handle, WM_CHAR, (WPARAM)comdat[i].Data[j], 0x110001 );
				j++;
			}while (comdat[i].Data[j]);
			LastCombineLen = j-1;
			LatestSent[0] = comdat[i].Data[LastCombineLen];
			LastIndex = -2;
			return true;
		}
	}
	return false;
}

void BackForCombination(){
	HWND handle = GetFocus();
	if (!handle)
		return;
	WillPost = LastCombineLen;
	while (LastCombineLen > 0){
		PostMessageW(handle, KM_CHAR, (WPARAM)VK_BACK, 0xE001);
		LastCombineLen--;
	}
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

	MSG* msg = (MSG*)lParam;
	if (msg->message == WM_KEYDOWN ||msg->message == WM_KEYUP &&
		msg->wParam == VK_LEFT || msg->wParam == VK_RIGHT){
			LastCombineLen = 0;
			LatestSent[0] = NULL;
	}
	if (msg->message == KM_CHAR){
		if (isActive == false)
			return CallNextHookEx(hGetMsgHook, nCode, wParam, lParam);

		if (WillPost == 0)
			return CallNextHookEx(hGetMsgHook, nCode, wParam, lParam);
		
		Index = CheckToOrder(msg->wParam);

		if (Index >= 0){

			if (OData[Index].isEnd == 1 && LastIndex == Index)
			{
				WillPost--;
				PostMessageW(msg->hwnd, WM_CHAR, msg->wParam, msg->lParam );
				LatestSent[0] = msg->wParam;
				return CallNextHookEx(hGetMsgHook, nCode, wParam, lParam);
			}

			if (OData[LastIndex].isEnd == 2)
			{
				WillPost--;
				PostMessageW(msg->hwnd, WM_CHAR, msg->wParam, msg->lParam );
				LatestSent[0] = NULL;
				return CallNextHookEx(hGetMsgHook, nCode, wParam, lParam);
			}

			or.Pattern((wchar_t*)OData[Index].Data, 9);

			LatestSent[1] = msg->wParam;
			bool isMatch = or.Match((wchar_t*)LatestSent,1+wcslen((wchar_t*)LatestSent));

			if (isMatch){
				LatestSent[0] = or.order(LatestSent);
				WillPost = 0;
				LastIndex = Index;
				if (!LatestSent[0]){
					Index = -1;
					return CallNextHookEx(hGetMsgHook, nCode, wParam, lParam);
				}

				Index = -1;
				return 1;
			}
			else {
				Index = -1;
			}
		}
		
		WillPost--;
		PostMessageW(msg->hwnd, WM_CHAR, msg->wParam, msg->lParam );
		LatestSent[0] = msg->wParam;
		LastIndex = -2;
	}

	return (hGetMsgHook, nCode, wParam, lParam);
}

int CheckToOrder(wchar_t KeyIn){

	Order CheckKey;
	for (int i=0;FileHeader->nOrdData > i; i++){
		CheckKey.Pattern(OData[i].Key, 9);
		if (CheckKey.Match((wchar_t*)&KeyIn,1))
		{
			return i;
		}
	}
	return -1;
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
		wsprintf((LPSTR)Msg, "\"%s\"\n is not a vaild KeyMagic's Keyboard file!", GetKeyBoard(Index));
		MessageBox(Commander_hWnd, (LPSTR)Msg , "KeyMagic", MB_ICONEXCLAMATION | MB_OK);
		isActive = false;
		return false;
	}
	data.vk = (wchar_t*)FilePtr+6;
	data.uc = (wchar_t*)FilePtr+6+(WORD)FileHeader->lvk;
	OData = (OrdData*)(FileHeader->luc+ data.uc);
	comdat = (CombineData*)((char*)OData+sizeof(OrdData)*FileHeader->nOrdData);

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

	for (int i=0,Length = lstrlen(&szKBNames[i]),j=0; j <= Index; i+=Length+1, j++,
		Length = lstrlen(&szKBNames[i])){
			GetPrivateProfileString(szKBP, (LPCSTR)&szKBNames[i], NULL, (LPSTR)szKBFile, 500, szINI);
	}

	lstrcpy(szINI,szDir);
	lstrcat(szINI, szKBFile);
	return szINI;
};

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