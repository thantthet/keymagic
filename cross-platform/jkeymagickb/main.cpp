#include <iostream>

#include "keyboard.h"

int main (int argc, char * const argv[]) {
    if (argc < 3) {
		std::cerr << "usage: " << argv[0] << " <KeyboardFile> <JSKeyboardFile>" << std::endl;
		return 1;
	}
	
	keyboard kb;
	kb.createJSKeyboard(argv[1], argv[2]);
    return 0;
}
