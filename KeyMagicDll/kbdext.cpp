//*******************************************************************
// Program: kbdext.c
// Source files: kbdext.c kbdext.h
// Author: Marc-André Moreau
// Last update: September 18th, 2008
// Description: Replacement API for Microsoft's ToUnicode() function
// You should load the current keyboard layout with loadKeyboardLayout()
// before calling convertVirtualKeyToWChar()
//*******************************************************************

#include "kbdext.h"

#include <stdio.h>

typedef PKBDTABLES(CALLBACK* KbdLayerDescriptor)(VOID);

/*PVK_TO_WCHARS1 pVkToWchars1 = NULL;
PVK_TO_WCHARS2 pVkToWchars2 = NULL;
PVK_TO_WCHARS3 pVkToWchars3 = NULL;
PVK_TO_WCHARS4 pVkToWchars4 = NULL;
PVK_TO_WCHARS5 pVkToWchars5 = NULL;
PVK_TO_WCHARS6 pVkToWchars6 = NULL;
PVK_TO_WCHARS7 pVkToWchars7 = NULL;
PVK_TO_WCHARS8 pVkToWchars8 = NULL;
PVK_TO_WCHARS9 pVkToWchars9 = NULL;
PVK_TO_WCHARS10 pVkToWchars10 = NULL;
PMODIFIERS pCharModifiers;
PDEADKEY pDeadKey;*/

PKBDTABLES pKbd;

HINSTANCE loadKeyboardLayout()
{
	HINSTANCE kbdLibrary;
	KbdLayerDescriptor pKbdLayerDescriptor = NULL;

	char layoutFile[MAX_PATH];
	char systemDirectory[MAX_PATH];
	char kbdLayoutFilePath[MAX_PATH];

	int i = 0;

	if(getKeyboardLayoutFile(layoutFile, sizeof(layoutFile)) == -1)
		return NULL;
		
	GetSystemDirectory(systemDirectory, MAX_PATH);

	sprintf_s(kbdLayoutFilePath, MAX_PATH, "%s\\%s", systemDirectory, layoutFile);

	kbdLibrary = LoadLibrary(kbdLayoutFilePath);

	pKbdLayerDescriptor = (KbdLayerDescriptor)GetProcAddress(kbdLibrary, "KbdLayerDescriptor");

	if(pKbdLayerDescriptor != NULL)
		pKbd = pKbdLayerDescriptor();
	else
		return NULL;


	/*do
	{
		INIT_PVK_TO_WCHARS(i, 1)
		INIT_PVK_TO_WCHARS(i, 2)
		INIT_PVK_TO_WCHARS(i, 3)
		INIT_PVK_TO_WCHARS(i, 4)
		INIT_PVK_TO_WCHARS(i, 5)
		INIT_PVK_TO_WCHARS(i, 6)
		INIT_PVK_TO_WCHARS(i, 7)
		INIT_PVK_TO_WCHARS(i, 8)
		INIT_PVK_TO_WCHARS(i, 9)
		INIT_PVK_TO_WCHARS(i, 10)
		i++;
	}
	while(pKbd->pVkToWcharTable[i].cbSize != 0);

	pCharModifiers = pKbd->pCharModifiers;
	pDeadKey = pKbd->pDeadKey;*/

	return kbdLibrary;
}

int unloadKeyboardLayout(HINSTANCE kbdLibrary)
{
	if(kbdLibrary != 0)
		return (FreeLibrary(kbdLibrary) != 0);
	else
		return 0;
}

UINT ScancodeToVirtualkey(UINT Scancode){
	if (Scancode < pKbd->bMaxVSCtoVK)
		return pKbd->pusVSCtoVK[Scancode];
	return 0;
}

/*int convertVirtualKeyToWChar(int virtualKey, PWCHAR outputChar, PWCHAR deadChar)
{
	int i = 0;
	short state = 0;
	int shift = -1;
	int mod = 0;
	int charCount = 0;
	int capsLock;

	WCHAR baseChar;
	WCHAR diacritic;
	*outputChar = 0;

	capsLock = (GetKeyState(VK_CAPITAL) & 0x1);

	do
	{
		state = GetAsyncKeyState(pCharModifiers->pVkToBit[i].Vk);

		if(pCharModifiers->pVkToBit[i].Vk == VK_SHIFT)
		shift = i + 1; // Get modification number for Shift key

		if(state & ~SHRT_MAX)
		{
			if(mod == 0)
				mod = i + 1;
			else
				mod = 0; // Two modifiers at the same time!
		}
		i++;
	}
	while(pCharModifiers->pVkToBit[i].Vk != 0);

	SEARCH_VK_IN_CONVERSION_TABLE(1)
	SEARCH_VK_IN_CONVERSION_TABLE(2)
	SEARCH_VK_IN_CONVERSION_TABLE(3)
	SEARCH_VK_IN_CONVERSION_TABLE(4)
	SEARCH_VK_IN_CONVERSION_TABLE(5)
	SEARCH_VK_IN_CONVERSION_TABLE(6)
	SEARCH_VK_IN_CONVERSION_TABLE(7)
	SEARCH_VK_IN_CONVERSION_TABLE(8)
	SEARCH_VK_IN_CONVERSION_TABLE(9)
	SEARCH_VK_IN_CONVERSION_TABLE(10)

	if(*deadChar != 0) // I see dead characters...
	{
		i = 0;
		do
		{
			baseChar = (WCHAR)pDeadKey[i].dwBoth;
			diacritic = (WCHAR)(pDeadKey[i].dwBoth >> 16);

			if((baseChar == *outputChar) && (diacritic == *deadChar))
			{
				*deadChar = 0;
				*outputChar = (WCHAR)pDeadKey[i].wchComposed;
			}
			i++;
		}
		while(pDeadKey[i].dwBoth != 0);
	}

	return charCount;
}*/

int getKeyboardLayoutFile(char* layoutFile, DWORD bufferSize)
{
	HKEY hKey;
	DWORD varType = REG_SZ;

	//char kbdName[KL_NAMELENGTH];
	//char kbdKeyPath[51 + KL_NAMELENGTH];

	//GetKeyboardLayoutName(kbdName);
	//kbdName = "00000409";

	//sprintf_s(kbdKeyPath, 51 + KL_NAMELENGTH,
	//	"SYSTEM\\CurrentControlSet\\Control\\Keyboard Layouts\\%s", kbdName);

	char kbdKeyPath[] = "SYSTEM\\CurrentControlSet\\Control\\Keyboard Layouts\\00000409";

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCTSTR)kbdKeyPath, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
        	return -1;

	if(RegQueryValueEx(hKey, "Layout File", NULL, &varType, (LPBYTE)layoutFile, &bufferSize) != ERROR_SUCCESS)
		return -1;

	RegCloseKey(hKey);

	return 1;
}
