/*
 *  tester.h
 *  KeyMagicTester
 *
 *  Created by Thant Thet Khin Zaw on 3/3/11.
 *  Copyright 2011 N/A. All rights reserved.
 *
 */

#include <string>
#include <fstream>
#include <vector>

#include <keymagic.h>

using namespace std;

class tester {
public:
	typedef vector<wstring> StringList;
	tester();
	bool test(wstring * in, wstring * out);
	bool beginTest(const char * file, const char * keyboardfile);
private:	
	StringList _stringList;
	KeyMagicEngine _kme;
	
	bool readfile(const char * file);
	wstring * UTF8toUTF16(const std::string& u8);
	//bool getKeyCodeAndModifier(int keyval, int * keycode, int * modifier);
};
