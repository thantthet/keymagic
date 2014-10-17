/*
 * KeyCodes.h
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

#ifndef KEYCODES_H_
#define KEYCODES_H_

#include <map>
#include <string>

namespace libkm {

/**
 * KeyCodes class can be used to translate between (so called) pre-defined into keycode values\n
 * @see getKeyValue()
 */
class KeyCodes {
public:

	enum KeyCodesIds {
	  id_VK_BACK = 2, id_VK_TAB, id_VK_RETURN, id_VK_SHIFT,
	  id_VK_CONTROL, id_VK_MENU, id_VK_PAUSE, id_VK_CAPITAL,
	  id_VK_KANJI, id_VK_ESCAPE, id_VK_SPACE, id_VK_PRIOR,
	  id_VK_NEXT, id_VK_DELETE, id_VK_KEY_0, id_VK_KEY_1,
	  id_VK_KEY_2, id_VK_KEY_3, id_VK_KEY_4, id_VK_KEY_5,
	  id_VK_KEY_6, id_VK_KEY_7, id_VK_KEY_8, id_VK_KEY_9,
	  id_VK_KEY_A, id_VK_KEY_B, id_VK_KEY_C, id_VK_KEY_D,
	  id_VK_KEY_E, id_VK_KEY_F, id_VK_KEY_G, id_VK_KEY_H,
	  id_VK_KEY_I, id_VK_KEY_J, id_VK_KEY_K, id_VK_KEY_L,
	  id_VK_KEY_M, id_VK_KEY_N, id_VK_KEY_O, id_VK_KEY_P,
	  id_VK_KEY_Q, id_VK_KEY_R, id_VK_KEY_S, id_VK_KEY_T,
	  id_VK_KEY_U, id_VK_KEY_V, id_VK_KEY_W, id_VK_KEY_X,
	  id_VK_KEY_Y, id_VK_KEY_Z, id_VK_NUMPAD0, id_VK_NUMPAD1,
	  id_VK_NUMPAD2, id_VK_NUMPAD3, id_VK_NUMPAD4, id_VK_NUMPAD5,
	  id_VK_NUMPAD6, id_VK_NUMPAD7, id_VK_NUMPAD8, id_VK_NUMPAD9,
	  id_VK_MULTIPLY, id_VK_ADD, id_VK_SEPARATOR, id_VK_SUBTRACT,
	  id_VK_DECIMAL, id_VK_DIVIDE, id_VK_F1, id_VK_F2,
	  id_VK_F3, id_VK_F4, id_VK_F5, id_VK_F6,
	  id_VK_F7, id_VK_F8, id_VK_F9, id_VK_F10,
	  id_VK_F11, id_VK_F12, id_VK_LSHIFT, id_VK_RSHIFT,
	  id_VK_LCONTROL, id_VK_RCONTROL, id_VK_LMENU, id_VK_RMENU,
	  id_VK_OEM_1, id_VK_OEM_PLUS, id_VK_OEM_COMMA, id_VK_OEM_MINUS,
	  id_VK_OEM_PERIOD, id_VK_OEM_2, id_VK_OEM_3, id_VK_OEM_4,
	  id_VK_OEM_5, id_VK_OEM_6, id_VK_OEM_7, id_VK_OEM_8,
	  id_VK_OEM_AX, id_VK_OEM_102, id_VK_ICO_HELP, id_VK_ICO_00
	};

	KeyCodes();
    
    static std::map<int, std::wstring> idKeyCode;
    
	/**
	 * Get the value of id
	 * @param id id
	 */
	static const std::wstring& getKeyValue(int id);
};

}

#endif /* KEYCODES_H_ */
