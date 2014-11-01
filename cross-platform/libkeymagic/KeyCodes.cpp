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

namespace libkm {
    
    std::map<int, std::wstring> KeyCodes::idKeyCode = {
        {KeyCodes::id_VK_BACK,      L"\x0008"},
        {KeyCodes::id_VK_TAB,       L"\x0009"},
        {KeyCodes::id_VK_RETURN,    L"\x000D"},
        {KeyCodes::id_VK_SHIFT,     L"\x0010"},
        {KeyCodes::id_VK_CONTROL,   L"\x0011"},
        {KeyCodes::id_VK_MENU,      L"\x0012"},
        {KeyCodes::id_VK_PAUSE,     L"\x0013"},
        {KeyCodes::id_VK_CAPITAL,   L"\x0014"},
        {KeyCodes::id_VK_KANJI,     L"\x0019"},
        {KeyCodes::id_VK_ESCAPE,    L"\x001B"},
        {KeyCodes::id_VK_SPACE,     L"\x0020"},
        {KeyCodes::id_VK_PRIOR,     L"\x0021"},
        {KeyCodes::id_VK_NEXT,      L"\x0022"},
        {KeyCodes::id_VK_DELETE,    L"\x002E"},
        
        {KeyCodes::id_VK_KEY_0, L"0"},
        {KeyCodes::id_VK_KEY_1, L"1"},
        {KeyCodes::id_VK_KEY_2, L"2"},
        {KeyCodes::id_VK_KEY_3, L"3"},
        {KeyCodes::id_VK_KEY_4, L"4"},
        {KeyCodes::id_VK_KEY_5, L"5"},
        {KeyCodes::id_VK_KEY_6, L"6"},
        {KeyCodes::id_VK_KEY_7, L"7"},
        {KeyCodes::id_VK_KEY_8, L"8"},
        {KeyCodes::id_VK_KEY_9, L"9"},
        
        {KeyCodes::id_VK_KEY_A, L"A"},
        {KeyCodes::id_VK_KEY_B, L"B"},
        {KeyCodes::id_VK_KEY_C, L"C"},
        {KeyCodes::id_VK_KEY_D, L"D"},
        {KeyCodes::id_VK_KEY_E, L"E"},
        {KeyCodes::id_VK_KEY_F, L"F"},
        {KeyCodes::id_VK_KEY_G, L"G"},
        {KeyCodes::id_VK_KEY_H, L"H"},
        {KeyCodes::id_VK_KEY_I, L"I"},
        {KeyCodes::id_VK_KEY_J, L"J"},
        {KeyCodes::id_VK_KEY_K, L"K"},
        {KeyCodes::id_VK_KEY_L, L"L"},
        {KeyCodes::id_VK_KEY_M, L"M"},
        {KeyCodes::id_VK_KEY_N, L"N"},
        {KeyCodes::id_VK_KEY_O, L"O"},
        {KeyCodes::id_VK_KEY_P, L"P"},
        {KeyCodes::id_VK_KEY_Q, L"Q"},
        {KeyCodes::id_VK_KEY_R, L"R"},
        {KeyCodes::id_VK_KEY_S, L"S"},
        {KeyCodes::id_VK_KEY_T, L"T"},
        {KeyCodes::id_VK_KEY_U, L"U"},
        {KeyCodes::id_VK_KEY_V, L"V"},
        {KeyCodes::id_VK_KEY_W, L"W"},
        {KeyCodes::id_VK_KEY_X, L"X"},
        {KeyCodes::id_VK_KEY_Y, L"Y"},
        {KeyCodes::id_VK_KEY_Z, L"Z"},
        
        {KeyCodes::id_VK_NUMPAD0, L"\x0060"},
        {KeyCodes::id_VK_NUMPAD1, L"\x0061"},
        {KeyCodes::id_VK_NUMPAD2, L"\x0062"},
        {KeyCodes::id_VK_NUMPAD3, L"\x0063"},
        {KeyCodes::id_VK_NUMPAD4, L"\x0064"},
        {KeyCodes::id_VK_NUMPAD5, L"\x0065"},
        {KeyCodes::id_VK_NUMPAD6, L"\x0066"},
        {KeyCodes::id_VK_NUMPAD7, L"\x0067"},
        {KeyCodes::id_VK_NUMPAD8, L"\x0068"},
        {KeyCodes::id_VK_NUMPAD9, L"\x0069"},
        
        {KeyCodes::id_VK_MULTIPLY,  L"\x006A"},
        {KeyCodes::id_VK_ADD,       L"\x006B"},
        {KeyCodes::id_VK_SEPARATOR, L"\x006C"},
        {KeyCodes::id_VK_SUBTRACT,  L"\x006D"},
        {KeyCodes::id_VK_DECIMAL,   L"\x006E"},
        {KeyCodes::id_VK_DIVIDE,    L"\x006F"},
        
        {KeyCodes::id_VK_F1, L"\x0070"},
        {KeyCodes::id_VK_F2, L"\x0071"},
        {KeyCodes::id_VK_F3, L"\x0072"},
        {KeyCodes::id_VK_F4, L"\x0073"},
        {KeyCodes::id_VK_F5, L"\x0074"},
        {KeyCodes::id_VK_F6, L"\x0075"},
        {KeyCodes::id_VK_F7, L"\x0076"},
        {KeyCodes::id_VK_F8, L"\x0077"},
        {KeyCodes::id_VK_F9, L"\x0078"},
        {KeyCodes::id_VK_F10, L"\x0079"},
        {KeyCodes::id_VK_F11, L"\x007A"},
        {KeyCodes::id_VK_F12, L"\x007B"},
        
        {KeyCodes::id_VK_LSHIFT, L"\x00A0"},
        {KeyCodes::id_VK_RSHIFT, L"\x00A1"},
        
        {KeyCodes::id_VK_LCONTROL, L"\x00A2"},
        {KeyCodes::id_VK_RCONTROL, L"\x00A3"},
        
        {KeyCodes::id_VK_LMENU, L"\x00A4"},
        {KeyCodes::id_VK_RMENU, L"\x00A5"},
        
        {KeyCodes::id_VK_OEM_1, L"\x00BA"},
        
        {KeyCodes::id_VK_OEM_PLUS,  L"\x00BB"},
        {KeyCodes::id_VK_OEM_COMMA, L"\x00BC"},
        {KeyCodes::id_VK_OEM_MINUS, L"\x00BD"},
        {KeyCodes::id_VK_OEM_PERIOD, L"\x00BE"},
        
        {KeyCodes::id_VK_OEM_2, L"\x00BF"},
        {KeyCodes::id_VK_OEM_3, L"\x00C0"},
        {KeyCodes::id_VK_OEM_4, L"\x00DB"},
        {KeyCodes::id_VK_OEM_5, L"\x00DC"},
        {KeyCodes::id_VK_OEM_6, L"\x00DD"},
        {KeyCodes::id_VK_OEM_7, L"\x00DE"},
        {KeyCodes::id_VK_OEM_8, L"\x00DF"},
        {KeyCodes::id_VK_OEM_AX, L"\x00E1"},
        {KeyCodes::id_VK_OEM_102, L"\x00E2"},

        {KeyCodes::id_VK_ICO_HELP, L"\x00E3"},
        {KeyCodes::id_VK_ICO_00, L"\x00E4"}
    };

}