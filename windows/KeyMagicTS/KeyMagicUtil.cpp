#include <ShlObj.h>
#include "KeyMagicUtil.h"
using namespace std;

KEYBOARD_LIST * SearchKeyboards(const wstring& dir) {
	wstring findPattern = wstring(dir);
	findPattern += L"*.km2";

	KEYBOARD_LIST * keyboards = new KEYBOARD_LIST();

	WIN32_FIND_DATAW fd;
	HANDLE hFind = FindFirstFileW(findPattern.c_str(), &fd);
	while (hFind  != INVALID_HANDLE_VALUE) {
		
		wstring keyboardLayout = wstring(dir) + wstring(fd.cFileName);
		keyboards->push_back(keyboardLayout);

		if (FindNextFileW(hFind, &fd) == FALSE) {
			break;
		}
	}
	return keyboards;
}

KEYBOARD_LIST * GetKeyboards() {
	wstring findDir;
	WCHAR keymagicDir[] = L"\\KeyMagic\\KeyboardLayouts\\";
	WCHAR path[MAX_PATH];

	KEYBOARD_LIST *keyboards,
		*allKeyboards = new KEYBOARD_LIST();

	SHGetFolderPathW(0, CSIDL_APPDATA, 0, 0, path);
	findDir = wstring(path) + wstring(keymagicDir);
	keyboards = SearchKeyboards(findDir);
	for (KEYBOARD_LIST::iterator i = keyboards->begin(); i != keyboards->end(); i++) {
		allKeyboards->push_back(*i);
	}
	delete keyboards;

	SHGetFolderPathW(0, CSIDL_COMMON_APPDATA, 0, 0, path);
	findDir = wstring(path) + wstring(keymagicDir);
	keyboards = SearchKeyboards(findDir);
	for (KEYBOARD_LIST::iterator i = keyboards->begin(); i != keyboards->end(); i++) {
		allKeyboards->push_back(*i);
	}
	delete keyboards;

	return allKeyboards;
}

BOOL MapVK (UINT inVK, UINT * VKey, UINT * Char) {

	BYTE keyboardState[256];
	UINT scanCode;
	WCHAR translatedChar[1];

	//HKL usHKL = GetKeyboardLayout(0);
	scanCode = MapVirtualKey(inVK, MAPVK_VK_TO_VSC);
	*VKey = MapVirtualKeyEx(scanCode, MAPVK_VSC_TO_VK, (HKL)0x4090409);

	if (scanCode == 0) {
		return false;
	}

	GetKeyboardState(keyboardState);

	int Return = ToUnicodeEx(*VKey, scanCode, keyboardState, translatedChar, 1, 0, (HKL)0x4090409);

	if (Return == 0) {
		*Char = '\0';
		return false;
	}

	*Char = translatedChar[0];

	return true;
}