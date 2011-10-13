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

#define KEYBOARD_FILE_NAME "MyanSan.km2"

#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
	char keyboardFile[] = "\\\\psf\\home\\codes\\Keymagic\\LayoutScripts\\" KEYBOARD_FILE_NAME;
#else
	char keyboardFile[] = "/Users/thantthetkz/codes/Keymagic/LayoutScripts/" KEYBOARD_FILE_NAME;
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
#ifdef _WIN32
	_CrtDumpMemoryLeaks();
#endif
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
		engine->processKeyEvent('*', '8', 1);
	}

    std::string s = getCharacterReferenceString(engine->getContextText());
    
    std::cout << s.c_str();
    
	delete engine;
#ifdef _WIN32
	_CrtDumpMemoryLeaks();
#endif
}

void print_hisotry(const TContextHistory &history) {
	std::string s;
    
	for (TContextHistory::const_iterator i = history.begin(); i != history.end(); i++) {
		s = getCharacterReferenceString(*i);
		std::cout << s.c_str();
	}
}

int
main (int argc, char *argv[]) {
//	leakCheck_processKeyEvent();
//	leakCheck_reLoadKeyboardFile();
    
    KeyMagicEngine * engine = new KeyMagicEngine();
    engine->loadKeyboardFile(keyboardFile);
    engine->m_verbose = true;
	
    unsigned char states[256] = {0};
	engine->setKeyStates(states);
    
    engine->processKeyEvent('u', 'U', 0);
    engine->processKeyEvent('f', 'F', 0);
    engine->processKeyEvent('h', 'H', 0);
    
    std::string s;
    
    s = getCharacterReferenceString(engine->getContextText());
    std::cout << s.c_str();
	
	print_hisotry(engine->getHistory());
    
    engine->processKeyEvent(8, 8, 0);
    
    s = getCharacterReferenceString(engine->getContextText());
    std::cout << s.c_str();
    
    engine->processKeyEvent(8, 8, 0);
    
    s = getCharacterReferenceString(engine->getContextText());
    std::cout << s.c_str();
    
    delete engine;
}