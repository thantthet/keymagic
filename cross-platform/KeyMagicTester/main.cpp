#include <iostream>
#include "tester.h"

int main (int argc, char * const argv[]) {
    tester _tester;
	
	if (argc == 1) {
		std::cerr << "usage: " << argv[0] << " <TestFile> <KeyboardFile>" << std::endl;
		return 1;
	} else if (argc == 2) {
		std::cerr << "usage: " << argv[0] << " " << argv[1] << " <KeyboardFile>" << std::endl;
		return 1;
	}
	
	if (_tester.beginTest(argv[1], argv[2])) {
		std::cout << "test success!" << std::endl;
	} else {
		std::cout << "test un-success!" << std::endl;
	}

    return 0;
}