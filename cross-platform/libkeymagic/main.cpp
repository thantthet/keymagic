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

#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
	char keyboardFile[] = "\\\\psf\\home\\codes\\Keymagic\\LayoutScripts\\Zawgyi L.km2";
#else
	char keyboardFile[] = "./../../../../LayoutScripts/Zawgyi L.km2";
#endif

// Check leaks if there any when loading new keyboard over existing one
// RESULT: Almost no leaks at the time of writting this comment :) . Now showing 4 byte leaks anyway, i can't find where is it?
void leakCheck_reLoadKeyboardFile() {
	KeyMagicEngine * engine = new KeyMagicEngine();

	bool b;
	for (int i = 0 ; i < 5; i++) {
		b = engine->loadKeyboardFile(keyboardFile);
	}

	delete engine;

	_CrtDumpMemoryLeaks();
}

// if leaks when processKeyEvent
// RESULT: no leaks
void leakCheck_processKeyEvent() {
	KeyMagicEngine * engine = new KeyMagicEngine();

	bool b;

	b = engine->loadKeyboardFile(keyboardFile);

	unsigned char states[256] = {0};

	engine->setKeyStates(states);

	for (int i = 0; i < 10; i++) {
		engine->processKeyEvent('u', 'U', 0);
	}

	delete engine;

	_CrtDumpMemoryLeaks();
}

int
main (int argc, char *argv[]) {
	leakCheck_processKeyEvent();
	leakCheck_reLoadKeyboardFile();
}