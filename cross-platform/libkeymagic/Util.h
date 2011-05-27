/*
 *  Util.h
 *  keymagic
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

#ifndef UTIL_H_
#define UTIL_H_


enum {
#ifndef VK_BACK
	VK_BACK = 0x0008,
	VK_TAB = 0x0009,
	VK_RETURN = 0x000D,
	VK_SHIFT = 0x0010,
	VK_CONTROL = 0x0011,
	VK_MENU = 0x0012,
	VK_PAUSE = 0x0013,
	VK_CAPITAL = 0x0014,
	VK_KANJI = 0x0019,
	VK_ESCAPE = 0x001B,
	VK_SPACE = 0x0020,
	VK_PRIOR = 0x0021,
	VK_NEXT = 0x0022,
	VK_DELETE = 0x002E,
#endif
	VK_KEY_0 = '0',
	VK_KEY_1 = '1',
	VK_KEY_2 = '2',
	VK_KEY_3 = '3',
	VK_KEY_4 = '4',
	VK_KEY_5 = '5',
	VK_KEY_6 = '6',
	VK_KEY_7 = '7',
	VK_KEY_8 = '8',
	VK_KEY_9 = '9',
	
	VK_KEY_A = 'A',
	VK_KEY_B = 'B',
	VK_KEY_C = 'C',
	VK_KEY_D = 'D',
	VK_KEY_E = 'E',
	VK_KEY_F = 'F',
	VK_KEY_G = 'G',
	VK_KEY_H = 'H',
	VK_KEY_I = 'I',
	VK_KEY_J = 'J',
	VK_KEY_K = 'K',
	VK_KEY_L = 'L',
	VK_KEY_M = 'M',
	VK_KEY_N = 'N',
	VK_KEY_O = 'O',
	VK_KEY_P = 'P',
	VK_KEY_Q = 'Q',
	VK_KEY_R = 'R',
	VK_KEY_S = 'S',
	VK_KEY_T = 'T',
	VK_KEY_U = 'U',
	VK_KEY_V = 'V',
	VK_KEY_W = 'W',
	VK_KEY_X = 'X',
	VK_KEY_Y = 'Y',
	VK_KEY_Z = 'Z',
#ifndef VK_NUMPAD0
	VK_NUMPAD0 = 0x0060,
	VK_NUMPAD1 = 0x0061,
	VK_NUMPAD2 = 0x0062,
	VK_NUMPAD3 = 0x0063,
	VK_NUMPAD4 = 0x0064,
	VK_NUMPAD5 = 0x0065,
	VK_NUMPAD6 = 0x0066,
	VK_NUMPAD7 = 0x0067,
	VK_NUMPAD8 = 0x0068,
	VK_NUMPAD9 = 0x0069,
	
	VK_MULTIPLY = 0x006A,
	VK_ADD = 0x006B,
	VK_SEPARATOR = 0x006C,
	VK_SUBTRACT = 0x006D,
	VK_DECIMAL = 0x006E,
	VK_DIVIDE = 0x006F,
	
	VK_F1 = 0x0070,
	VK_F2 = 0x0071,
	VK_F3 = 0x0072,
	VK_F4 = 0x0073,
	VK_F5 = 0x0074,
	VK_F6 = 0x0075,
	VK_F7 = 0x0076,
	VK_F8 = 0x0077,
	VK_F9 = 0x0078,
	VK_F10 = 0x0079,
	VK_F11 = 0x007A,
	VK_F12 = 0x007B,
	
	VK_LSHIFT = 0x00A0,
	VK_RSHIFT = 0x00A1,
	
	VK_LCONTROL = 0x00A2,
	VK_RCONTROL = 0x00A3,
	
	VK_LMENU = 0x00A4,
	VK_RMENU = 0x00A5,
	
	VK_OEM_1 = 0x00BA,
	VK_OEM_PLUS = 0x00BB,
	VK_OEM_COMMA = 0x00BC,
	VK_OEM_MINUS = 0x00BD,
	VK_OEM_PERIOD = 0x00BE,
	VK_OEM_2 = 0x00BF,
	VK_OEM_3 = 0x00C0,
	VK_OEM_4 = 0x00DB,
	VK_OEM_5 = 0x00DC,
	VK_OEM_6 = 0x00DD,
	VK_OEM_7 = 0x00DE,
	VK_OEM_8 = 0x00DF,
	
	VK_OEM_AX = 0x00E1,
	
	VK_OEM_102 = 0x00E2,
	
	VK_ICO_HELP = 0x00E3,
	VK_ICO_00 = 0x00E4
#endif
};

/**
 * Convert keycode and modifier value from keyval. eg. A[keyval] = A[keycode] SHIFT_MASK[modifier]
 * @param keyval Key value to convert
 * @param keycode pointer to receive keycode value
 * @param modifier pointer to receive modifier value
 * @return true if keyval was converted else false
 */
bool GetKeyCodeAndModifier(int keyval, int * keycode, int * modifier);

#if defined(_WIN32)
InfoList * GetInfosFromKeyboardFile(const char * file);
#endif

#endif