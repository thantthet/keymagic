#include <iostream>
#include "keymagic-driver.hpp"

int
main (int argc, char *argv[])
{
	keymagic_driver driver;
	//driver.trace_parsing = true;
	//driver.parse ("test.txt");
	//driver.CreateLayoutFile();
	//return 0;

	if (argc < 2) {
		std::cout << argv[0] << " [-ps] kms_file [output_file]" << std::endl;
		std::cout << "-p\t\ttrace parsing" << std::endl;
		std::cout << "-s\t\ttrace scanning" << std::endl;
		std::cout << "kms_file\tkms file to compile" << std::endl;
		std::cout << "output_file\tcompiled output file name.\n\t\tIf not specified, no output would be generated"; 
	}

	for (++argv; argv[0]; ++argv) {
		if (*argv == std::string ("-p"))
			driver.trace_parsing = true;
		else if (*argv == std::string ("-s"))
			driver.trace_scanning = true;
		else if (driver.parse (*argv) == 0) {
			std::cout << "Successfully parsed!" << std::endl;
			++argv;
			if (argv[0]) {
				std::cout << "Creating Layout File..." << std::endl;
				driver.CreateLayoutFile(*argv);
				std::cout << "File created successfully." << std::endl;
			}
			return 0;
		} else {
			return 1;
		}
	}
}
