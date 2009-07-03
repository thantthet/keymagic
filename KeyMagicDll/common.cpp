#include "common.h"

#pragma data_seg(".keymagic")
HHOOK hKeyHook = NULL;
HHOOK hWndProcHook = NULL;
HHOOK hGetMsgHook = NULL;
HWND hwndKWindows = NULL;
char szDir[1000] = {0};
#pragma data_seg()

#define DEBUG 1

//Make sure that section can READ WRITE and SHARE
#pragma comment(linker, "/SECTION:.keymagic,RWS")

bool LoadKeymapFile(int index)
{
	char szKBPath[MAX_PATH];
	if (GetKeyBoard(index, szKBPath))
		return klf.fromFile(szKBPath);
	return false;
}

void GetShortCuts(){
//	Logger("GetShortCuts Entry");
	char szINI[MAX_PATH]={0};
	char szKBNames[MAX_PATH]={0};
	char szKBP[] ="KeyBoardPaths";
	char szSC[] ="ShortCuts";

	lstrcpy(szINI,szDir);
	lstrcat(szINI,"\\KeyMagic.ini");

	GetPrivateProfileString(szKBP, NULL, NULL, szKBNames, 500, szINI);

	//vtSC = (KM_ShortCut*) LocalAlloc(LPTR, sizeof(KM_ShortCut)*50);

	//NumOfShortCut=0;
	for (int i=0,Length = lstrlen(&szKBNames[i]);
		Length > 0;
		i+=Length+1, Length = lstrlen(&szKBNames[i])){
			WORD Hotkey = GetPrivateProfileInt(szSC, &szKBNames[i], 0, szINI);
			if (Hotkey){
			//	vtSC[j].ukey = (char) Hotkey;
			//	vtSC[j].modkey = Hotkey >> 8;
				KM_ShortCut * SC = new KM_ShortCut;
				SC->ukey = (char) Hotkey;
				SC->modkey = Hotkey >> 8;

				vtSC.push_back(*SC);
			}
			//NumOfShortCut++;
	}

	//LocalReAlloc(vtSC, sizeof(KM_ShortCut)*NumOfShortCut, LPTR);

//	Logger("GetShortCuts Return : NumOfShortCut = %X ", NumOfShortCut);
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

	OutputDebugStringA(Memory);
	
	//Cleanup
	va_end(list);
#else
	return;
#endif
}

LPCSTR GetKeyBoard(UINT Index, char * szKBPath){
	char szINI[MAX_PATH];
	char szKBNames[500];
	char szKBFile[MAX_PATH];
	char szAllUser[MAX_PATH];
	char szKBP[] = "KeyBoardPaths";

	lstrcpy(szINI,szDir);
	PathAppend(szINI, "KeyMagic.ini");

	GetPrivateProfileString(szKBP, NULL, NULL, (LPSTR)szKBNames, 500, szINI);

	for (int i=0,Length = lstrlen(&szKBNames[i]),j=0; 
		j <= Index;
		i+=Length+1, j++, Length = lstrlen(&szKBNames[i])){
			GetPrivateProfileString(szKBP, (LPCSTR)&szKBNames[i], NULL, (LPSTR)szKBFile, MAX_PATH, szINI);
	}

	if (szKBFile[1] == ':')
	{
		lstrcpy(szKBPath, szKBFile);
		return szKBPath;
	}
	
	lstrcpy(szKBPath, szDir);
	PathAppend(szKBPath, szKBFile);

	return szKBPath;
}

bool TranslateToAscii (UINT *uVKey){
//	Logger("TranslateToAscii Entry");

	//bool shiftDown;// = GetKeyState(VK_SHIFT) & 0x8000;
	//bool capsToggled;// = GetKeyState(VK_CAPITAL) & 0x1;
	//bool isUp = false;

	BYTE KeyStates[256];

	GetKeyboardState(KeyStates);
	KeyStates[VK_CONTROL]=KeyStates[VK_MENU]=KeyStates[VK_LMENU]=KeyStates[VK_RMENU]=0;

	//shiftDown = KeyStates[VK_SHIFT] & 0x80;
	//capsToggled = KeyStates[VK_CAPITAL] & 0x1;
	

//	Logger("UpperAndLower : isUp = %X", isUp);

 	WORD TransedChar = NULL;
	UINT ScanCode = MapVirtualKey(*uVKey, MAPVK_VK_TO_VSC);
	//TransedChar = MapVirtualKey(*uVKey, MAPVK_VK_TO_CHAR);

//	Logger("uVKey = %X", *uVKey);

	if (!ScanCode)
		return false;

	int Return = ToAscii(*uVKey, ScanCode, KeyStates, &TransedChar, 0);

	if (!Return)
		return false;

//	Logger("uVKey = %X TransedChar = %X Return = %X ScanCode = %X", *uVKey, TransedChar, Return, ScanCode);
	if (TransedChar > 33 || TransedChar < 126)
		*uVKey = TransedChar;
	else
		return false;

//	Logger("TranslateToAscii Return");
	return true;
}