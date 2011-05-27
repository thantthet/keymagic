/*
 * KeyCodes.cpp
 *
 * Copyright (C) 2008  KeyMagic Project
 * http://keymagic.googlecode.com
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "KeyCodes.h"

KeyCodes::KeyCodes() {

	idKeyCode[id_VK_BACK] = L"\x0008";
	idKeyCode[id_VK_TAB] = L"\x0009";
	idKeyCode[id_VK_RETURN] = L"\x000D";
	idKeyCode[id_VK_SHIFT] = L"\x0010";
	idKeyCode[id_VK_CONTROL] = L"\x0011";
	idKeyCode[id_VK_MENU] = L"\x0012";
	idKeyCode[id_VK_PAUSE] = L"\x0013";
	idKeyCode[id_VK_CAPITAL] = L"\x0014";
	idKeyCode[id_VK_KANJI] = L"\x0019";
	idKeyCode[id_VK_ESCAPE] = L"\x001B";
	idKeyCode[id_VK_SPACE] = L"\x0020";
	idKeyCode[id_VK_PRIOR] = L"\x0021";
	idKeyCode[id_VK_NEXT] = L"\x0022";
	idKeyCode[id_VK_DELETE] = L"\x002E";

	idKeyCode[id_VK_KEY_0] = L"0";
	idKeyCode[id_VK_KEY_1] = L"1";
	idKeyCode[id_VK_KEY_2] = L"2";
	idKeyCode[id_VK_KEY_3] = L"3";
	idKeyCode[id_VK_KEY_4] = L"4";
	idKeyCode[id_VK_KEY_5] = L"5";
	idKeyCode[id_VK_KEY_6] = L"6";
	idKeyCode[id_VK_KEY_7] = L"7";
	idKeyCode[id_VK_KEY_8] = L"8";
	idKeyCode[id_VK_KEY_9] = L"9";

	idKeyCode[id_VK_KEY_A] = L"A";
	idKeyCode[id_VK_KEY_B] = L"B";
	idKeyCode[id_VK_KEY_C] = L"C";
	idKeyCode[id_VK_KEY_D] = L"D";
	idKeyCode[id_VK_KEY_E] = L"E";
	idKeyCode[id_VK_KEY_F] = L"F";
	idKeyCode[id_VK_KEY_G] = L"G";
	idKeyCode[id_VK_KEY_H] = L"H";
	idKeyCode[id_VK_KEY_I] = L"I";
	idKeyCode[id_VK_KEY_J] = L"J";
	idKeyCode[id_VK_KEY_K] = L"K";
	idKeyCode[id_VK_KEY_L] = L"L";
	idKeyCode[id_VK_KEY_M] = L"M";
	idKeyCode[id_VK_KEY_N] = L"N";
	idKeyCode[id_VK_KEY_O] = L"O";
	idKeyCode[id_VK_KEY_P] = L"P";
	idKeyCode[id_VK_KEY_Q] = L"Q";
	idKeyCode[id_VK_KEY_R] = L"R";
	idKeyCode[id_VK_KEY_S] = L"S";
	idKeyCode[id_VK_KEY_T] = L"T";
	idKeyCode[id_VK_KEY_U] = L"U";
	idKeyCode[id_VK_KEY_V] = L"V";
	idKeyCode[id_VK_KEY_W] = L"W";
	idKeyCode[id_VK_KEY_X] = L"X";
	idKeyCode[id_VK_KEY_Y] = L"Y";
	idKeyCode[id_VK_KEY_Z] = L"Z";

	idKeyCode[id_VK_NUMPAD0] = L"\x0060";
	idKeyCode[id_VK_NUMPAD1] = L"\x0061";
	idKeyCode[id_VK_NUMPAD2] = L"\x0062";
	idKeyCode[id_VK_NUMPAD3] = L"\x0063";
	idKeyCode[id_VK_NUMPAD4] = L"\x0064";
	idKeyCode[id_VK_NUMPAD5] = L"\x0065";
	idKeyCode[id_VK_NUMPAD6] = L"\x0066";
	idKeyCode[id_VK_NUMPAD7] = L"\x0067";
	idKeyCode[id_VK_NUMPAD8] = L"\x0068";
	idKeyCode[id_VK_NUMPAD9] = L"\x0069";

	idKeyCode[id_VK_MULTIPLY] = L"\x006A";
	idKeyCode[id_VK_ADD] = L"\x006B";
	idKeyCode[id_VK_SEPARATOR] = L"\x006C";
	idKeyCode[id_VK_SUBTRACT] = L"\x006D";
	idKeyCode[id_VK_DECIMAL] = L"\x006E";
	idKeyCode[id_VK_DIVIDE] = L"\x006F";

	idKeyCode[id_VK_F1] = L"\x0070";
	idKeyCode[id_VK_F2] = L"\x0071";
	idKeyCode[id_VK_F3] = L"\x0072";
	idKeyCode[id_VK_F4] = L"\x0073";
	idKeyCode[id_VK_F5] = L"\x0074";
	idKeyCode[id_VK_F6] = L"\x0075";
	idKeyCode[id_VK_F7] = L"\x0076";
	idKeyCode[id_VK_F8] = L"\x0077";
	idKeyCode[id_VK_F9] = L"\x0078";
	idKeyCode[id_VK_F10] = L"\x0079";
	idKeyCode[id_VK_F11] = L"\x007A";
	idKeyCode[id_VK_F12] = L"\x007B";

	idKeyCode[id_VK_LSHIFT] = L"\x00A0";
	idKeyCode[id_VK_RSHIFT] = L"\x00A1";

	idKeyCode[id_VK_LCONTROL] = L"\x00A2";
	idKeyCode[id_VK_RCONTROL] = L"\x00A3";

	idKeyCode[id_VK_LMENU] = L"\x00A4";
	idKeyCode[id_VK_RMENU] = L"\x00A5";

	idKeyCode[id_VK_OEM_1] = L"\x00BA";

	idKeyCode[id_VK_OEM_PLUS] = L"\x00BB";
	idKeyCode[id_VK_OEM_COMMA] = L"\x00BC";
	idKeyCode[id_VK_OEM_MINUS] = L"\x00BD";
	idKeyCode[id_VK_OEM_PERIOD] = L"\x00BE";

	idKeyCode[id_VK_OEM_2] = L"\x00BF";

	idKeyCode[id_VK_OEM_3] = L"\x00C0";

	idKeyCode[id_VK_OEM_4] = L"\x00DB";

	idKeyCode[id_VK_OEM_5] = L"\x00DC";

	idKeyCode[id_VK_OEM_6] = L"\x00DD";

	idKeyCode[id_VK_OEM_7] = L"\x00DE";

	idKeyCode[id_VK_OEM_8] = L"\x00DF";

	idKeyCode[id_VK_OEM_AX] = L"\x00E1";

	idKeyCode[id_VK_OEM_102] = L"\x00E2";

	idKeyCode[id_VK_ICO_HELP] = L"\x00E3";
	idKeyCode[id_VK_ICO_00] = L"\x00E4";
}

const std::wstring& KeyCodes::getKeyValue(int id) {
	return idKeyCode[id];
}
