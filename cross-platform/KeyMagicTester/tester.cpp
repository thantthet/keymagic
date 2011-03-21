/*
 *  tester.cpp
 *  KeyMagicTester
 *
 *  Created by Thant Thet Khin Zaw on 3/3/11.
 *  Copyright 2011 N/A. All rights reserved.
 *
 */

#include "tester.h"
#include "ConvertUTF.h"

using namespace std;

tester::tester()
{
}

bool tester::test(wstring * in, wstring * out) {
	_kme.reset();
	for (wstring::iterator i = in->begin(); i != in->end(); i++) {
		int keyval = *i;
		int keycode, modifier;
		getKeyCodeAndModifier(keyval, &keycode, &modifier);
		_kme.processKeyEvent(keyval, keycode, modifier);
	}
	KeyMagicString * context = _kme.getContextText();
	if (out->compare(context->c_str()) != 0) {
		return false;
	}
	return true;
}

bool tester::beginTest(const char * file, const char * keyboardfile)
{
	if (readfile(file) == false) {
		cerr << "Failed to read test file." << std::endl;
		return false;
	}

	KeyMagicKeyboard::getInfosFromKeyboardFile(keyboardfile);

	if (_kme.loadKeyboardFile(keyboardfile) == false) {
		cerr << "Failed to load keyboard layout file." << std::endl;
		return false;
	}
	_kme.loadKeyboardFile(keyboardfile);
	_kme.loadKeyboardFile(keyboardfile);

	const InfoList infos = _kme.getKeyboard()->getInfoList();
	if (infos.find('name') != infos.end()) {
		cout << "Testing with Keyboard Layout: " << infos.find('name')->second.data << std::endl;
	}
	for (StringList::iterator i = _stringList.begin(); i != _stringList.end(); i++) {
		if ((i+1) != _stringList.end()) {
			wstring * in = &*i++;
			wstring * out = &*i;
			string * s = getCharacterReferenceString(out);
			if (test(in, out)) {
				std::wcout << *in << " => " << s->c_str() << endl;
			} else {
				string * ss = getCharacterReferenceString(_kme.getContextText());
				std::wcout << *in << " !=> " << s->c_str() << " [" << ss->c_str() << "]" << endl;
				delete ss;
			}
			delete s;

		}
	}
	
	return true;
}

bool tester::readfile(const char * file)
{
	ifstream _fstream;
	_fstream.open(file);
	
	if (_fstream.is_open() == false)
		return false;
	
	while (!_fstream.eof()) {
		char line[1000];
		_fstream.getline(line, sizeof(line));
		wstring * widestr = UTF8toUTF16(line);
		_stringList.push_back(*widestr);
		delete widestr;
	}
	
	return true;
}

wstring * tester::UTF8toUTF16(const std::string& u8)
{
	unsigned int length = u8.length();
	length++;
	
	const UTF8 * source = (UTF8*)u8.c_str();
	const UTF8 * sourceStart = source;
	const UTF8 * sourceEnd = source + length;
	
	UTF16 * target = new UTF16[length];
	UTF16 * targetStart = target;
	UTF16 * targetEnd = target + length;
	
	ConvertUTF8toUTF16(&sourceStart, sourceEnd, &targetStart, targetEnd, lenientConversion);
	
	wchar_t * wcs = new wchar_t[length];
	memset(wcs, 0, length * sizeof (wchar_t));
	for (int i = 0; i < targetStart - target; i++) {
		wcs[i] = target[i];
	}
	
	std::wstring * s = new std::wstring(wcs);
	
	delete[] wcs;
	delete[] target;
	
	return s;
}