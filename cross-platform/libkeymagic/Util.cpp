/*
 *  Util.cpp
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

#include "KeyMagicTypes.h"
#include "KeyMagicKeyboard.h"
#include "KeyMagicEngine.h"
#include "Util.h"

#define CASE(a,b,c) case a:case b: *keycode = c;break

bool GetKeyCodeAndModifier(int keyval, int * keycode, int * modifier)
{
	if (isalpha(keyval)) {
		if (isupper(keyval)) {
			*keycode = keyval;
			*modifier = KeyMagicEngine::SHIFT_MASK;
		} else {
			*keycode = keyval - ('a' - 'A');
			*modifier = 0;
		}
		return true;
	}
	switch (keyval) {
		CASE('~','`',VK_OEM_3);
		CASE('!','1',VK_KEY_1);
		CASE('@','2',VK_KEY_2);
		CASE('#','3',VK_KEY_3);
		CASE('$','4',VK_KEY_4);
		CASE('%','5',VK_KEY_5);
		CASE('^','6',VK_KEY_6);
		CASE('&','7',VK_KEY_7);
		CASE('*','8',VK_KEY_8);
		CASE('(','9',VK_KEY_9);
		CASE(')','0',VK_KEY_0);
		CASE('_','-',VK_OEM_MINUS);
		CASE('+','=',VK_OEM_PLUS);
		CASE('{','[',VK_OEM_4);
		CASE('}',']',VK_OEM_6);
		CASE('|','\\',VK_OEM_5);
		CASE(':',';',VK_OEM_1);
		CASE('"','\'',VK_OEM_7);
		CASE('<',',',VK_OEM_COMMA);
		CASE('>','.',VK_OEM_PERIOD);
		CASE('?','/',VK_OEM_2);
		default:
			*keycode = 0;
			*modifier = 0;
			return false;
			break;
	}
	std::string shifted("~!@#$%^&*()_+{}|:\"<>?");
	if (shifted.find(keyval, 0) != std::string::npos) {
		*modifier = KeyMagicEngine::SHIFT_MASK;
	} else {
		*modifier = 0;
	}

	
	return true;
}

#if defined(_WIN32)

InfoList * GetInfosFromKeyboardFile(const char * file) {
	return KeyMagicKeyboard::getInfosFromKeyboardFile(file);
}

#endif