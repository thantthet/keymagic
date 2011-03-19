/*
 *  Util.cpp
 *  keymagic
 *
 *  Created by Thant Thet Khin Zaw on 3/3/11.
 *  Copyright 2011 N/A. All rights reserved.
 *
 */

#include "Util.h"
#include "KeyMagicEngine.h"

#define CASE(a,b,c) case a:case b: *keycode = c;break

bool getKeyCodeAndModifier(int keyval, int * keycode, int * modifier)
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