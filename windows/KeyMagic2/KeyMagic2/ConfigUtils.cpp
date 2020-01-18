#include "stdafx.h"
#include "ConfigUtils.h"
#include <ShellApi.h>

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

LPCSTR ConfigKeyKeyboards = "keyboards";
LPCSTR ConfigKeyHotkeys = "hotkeys";
LPCSTR ConfigKeyHotkeysOnOff = "onoff";
LPCSTR ConfigKeyHotkeysNext = "next";
LPCSTR ConfigKeyKeyboardPerWindow = "keyboardPerWindow";

int SHRemoveDirectory(LPCTSTR dir)
{
	SHFILEOPSTRUCT file_op = {
		NULL,
		FO_DELETE,
		dir,
		_T(""),
		FOF_NOCONFIRMATION |
		FOF_NOERRORUI |
		FOF_SILENT |
		FOF_ALLOWUNDO,
		false,
		0,
		_T("") };
	return SHFileOperation(&file_op);
}