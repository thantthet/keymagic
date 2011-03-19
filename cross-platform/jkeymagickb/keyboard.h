/*
 *  keyboard.h
 *  jkeymagickb
 *
 *  Created by Thant Thet Khin Zaw on 3/8/11.
 *  Copyright 2011 N/A. All rights reserved.
 *
 */

#include "libkeymagic/KeyMagicKeyboard.h"

class keyboard : public KeyMagicKeyboard {
public:
	keyboard();
	bool createJSKeyboard(const char * keyboardFileName, const char * jsKeyboardFileName);
};