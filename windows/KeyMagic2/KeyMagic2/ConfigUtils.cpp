#include "stdafx.h"
#include "ConfigUtils.h"

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

LPCSTR ConfigKeyKeyboards = "keyboards";
LPCSTR ConfigKeyHotkeys = "hotkeys";
LPCSTR ConfigKeyHotkeysOnOff = "onoff";
LPCSTR ConfigKeyHotkeysNext = "next";
LPCSTR ConfigKeyKeyboardPerWindow = "keyboardPerWindow";

BOOL DirectoryExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

std::wstring AppDataDirectory(BOOL roaming, BOOL create)
{
	TCHAR szPath[MAX_PATH];
	std::wstring destDirPath;

	if (SUCCEEDED(SHGetFolderPath(NULL, roaming ? CSIDL_APPDATA : CSIDL_LOCAL_APPDATA, NULL, 0, szPath))) {
		destDirPath = szPath;
		destDirPath += _T("\\KeyMagic\\");
		CreateDirectory(destDirPath.c_str(), NULL);
	}

	return destDirPath;
}