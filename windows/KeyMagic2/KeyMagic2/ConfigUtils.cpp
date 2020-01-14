#include "stdafx.h"
#include "ConfigUtils.h"

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

LPCSTR ConfigKeyKeyboards = "keyboards";
LPCSTR ConfigKeyHotkeys = "hotkeys";
LPCSTR ConfigKeyHotkeysOnOff = "onoff";
LPCSTR ConfigKeyHotkeysNext = "next";
LPCSTR ConfigKeyKeyboardPerWindow = "keyboardPerWindow";