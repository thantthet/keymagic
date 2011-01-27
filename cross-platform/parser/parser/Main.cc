#include <iostream>
#include "keymagic-driver.hh"

#pragma warning (disable: 4996)

int
main (int argc, char *argv[])
{
	keymagic_driver driver;
	//driver.trace_parsing = true;
	//driver.parse ("test.txt");
	//driver.CreateLayoutFile();
	//return 0;
	for (++argv; argv[0]; ++argv) {
		if (*argv == std::string ("-p"))
			driver.trace_parsing = true;
		else if (*argv == std::string ("-s"))
			driver.trace_scanning = true;
		else if (driver.parse (*argv) == 0) {
			std::cout << "Successfully parsed!" << std::endl;
			++argv;
			if (argv[0]) {
				std::cout << "Creating Layout file..." << std::endl;
				if (driver.CreateLayoutFile(*argv) == true) {
					std::cout << "Layout file is created successfully." << std::endl;
				} else {
					std::cerr << "Failed to open file :" << *argv << std::endl;
					return 1;
				}
			}
			return 0;
		} else {
			return 1;
		}
	}
}
