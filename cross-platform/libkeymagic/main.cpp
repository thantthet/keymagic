/*
 *  main.cpp
 *  keymagic
 *
 *  Created by Thant Thet Khin Zaw on 6/13/11.
 *  Copyright 2011 N/A. All rights reserved.
 *
 */

#include "keymagic.h"

using namespace libkm;

int
main (int argc, char *argv[]) {
	KeyMagicEngine engine;
	bool b = engine.loadKeyboardFile("../../../../LayoutScripts/Zawgyi L - Unicode.km2");
	if (b) {
		//engine.processInput('u', 'U', 0);
		
		unsigned int del = 0;
		KeyMagicString str;
		
		getDifference(L"", L"a", &del, &str);
		
		PERROR("%d", del);
	}
}