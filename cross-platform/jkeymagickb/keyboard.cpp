/*
 *  keyboard.cpp
 *  jkeymagickb
 *
 *  Created by Thant Thet Khin Zaw on 3/8/11.
 *  Copyright 2011 N/A. All rights reserved.
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>

#include "keyboard.h"
#include "ConvertUTF.h"

keyboard::keyboard() : KeyMagicKeyboard() {
}

std::string * UTF16toUTF8(const std::wstring& u16)
{
	unsigned int length = u16.length() * 4;
	length++;
	
	UTF16 * source = new UTF16[length+1];//(UTF16*)u16.c_str();
	memset(source, 0, length+1 * sizeof (UTF16));
	const UTF16 * sourceStart = source;
	const UTF16 * sourceEnd = source + u16.length();
	
	for (std::wstring::const_iterator i = u16.begin(); i != u16.end(); i++) {
		source[i-u16.begin()] = *i;
	}
	
	UTF8 * target = new UTF8[length+1];
	UTF8 * targetStart = target;
	UTF8 * targetEnd = target + length;

	ConversionResult r = ConvertUTF16toUTF8(&sourceStart, sourceEnd, &targetStart, targetEnd, lenientConversion);
	if (r != conversionOK) {
		std::cerr << r << "=!"<< conversionOK <<"\n";
	}
	
	char * cs = new char[length+1];
	memset(cs, 0, length+1 * sizeof (char));
	for (int i = 0; i < targetStart - target; i++) {
		cs[i] = target[i];
	}
	
	std::string * s = new std::string(cs);
	
	delete[] source;
	delete[] cs;
	delete[] target;
	
	return s;
}

std::string * JSONforRule(std::vector<RuleInfo::Item*> * rule) {
	std::stringstream ss;
	
	for (std::vector<RuleInfo::Item*>::iterator i = rule->begin(); i != rule->end(); i++) {
		RuleInfo::Item * item = *i;
		std::string * s;
		
		if (i != rule->begin()) {
			ss << ",\n";
		}
		
		switch (item->type) {
			case RuleInfo::tString:
				s = getCharacterReferenceString(item->stringValue);
				ss << "{'type':tString, 'value':'" << s->c_str() << "'}";
				delete s;
				break;
			case RuleInfo::tAnyOfString:
				s = getCharacterReferenceString(item->stringValue);
				ss << "{'type':tAnyOfString, 'value':'" << s->c_str() << "'}";
				delete s;
				break;
			case RuleInfo::tNotOfString:
				s = getCharacterReferenceString(item->stringValue);
				ss << "{'type':tNotOfString, 'value':'" << s->c_str() << "'}";
				delete s;
				break;
			case RuleInfo::tBackRefString:
				s = getCharacterReferenceString(item->stringValue);
				ss << "{'type':tBackRefString, 'index':" << std::dec << item->refIndex << ", 'value':'" << s->c_str() << "'}";
				delete s;
				break;
			case RuleInfo::tReference:
				ss << "{'type':tReference, 'index':" << std::dec << item->refIndex << "}";
				break;
			case RuleInfo::tVKey:
				ss << "{'type':tVKey, 'keyCode':0x" << std::hex << item->keyCode << "}";
				break;
			case RuleInfo::tAny:
				ss << "{'type':tAny}";
				break;
			case RuleInfo::tSwitch:
				ss << "{'type':tSwitch, 'switchId':" << std::dec << item->switchId << "}";
				break;
			default:
				ss << "\t{ type=tUnknown }";
				break;
		}
	}
	std::string * json = new std::string(ss.str());
	return json;
}

bool keyboard::createJSKeyboard(const char * keyboardFileName, const char * jsKeyboardFileName) {
	if (!KeyMagicKeyboard::loadKeyboardFile(keyboardFileName)) {
		return false;
	}
	
	std::ofstream fs;
	fs.open(jsKeyboardFileName);
	if (!fs.good()) {
		return false;
	}
	
	char * hotkey = 0,
		*kbTitle = new char[strlen(keyboardFileName)];
	
	//get file title
	int i;
	for (i = strlen(keyboardFileName); i >= 0; i--) {
		if (keyboardFileName[i] == '/' || keyboardFileName[i] == '\\') break;
	}
	strcpy(kbTitle, &keyboardFileName[++i]);
	i = strlen(kbTitle);
	while (kbTitle[--i] != '.') kbTitle[i] = '\0';
	kbTitle[i] = '\0';
	
	LayoutOptions * options = KeyMagicKeyboard::getLayoutOptions();
	InfoList infos = KeyMagicKeyboard::getInfoList();
	
	InfoList::iterator htky = infos.find('htky');
	if (htky != infos.end()) {
		hotkey = new char[htky->second.size + 1];
		strncpy(hotkey, htky->second.data, htky->second.size);
		hotkey[htky->second.size] = 0;
	}
	
	fs << "var keyboard =" << "{\n";
	fs << "'name':" << "'" << kbTitle << "',\n";
	fs << "'autobksp':" << std::boolalpha << options->autoBksp << ",\n";
	fs << "'trackcaps':" << std::boolalpha << options->trackCaps << ",\n";
	fs << "'eat':" << std::boolalpha << options->eat << ",\n";
	if (hotkey) fs << "'hotkey': '" << hotkey << "',\n";
	fs << "'rules': [\n";
	
	RuleList * rules = KeyMagicKeyboard::getRules();
	for (RuleList::iterator i = rules->begin(); i != rules->end(); i++) {
		RuleInfo * rule = *i;
		std::string * json;
		if (i != rules->begin()) {
			fs << ",\n";
		}
		fs << "{\n";
		fs << "'length':" << rule->getMatchLength() << ",\n";
		std::vector<RuleInfo::Item*> * lhs = rule->getLHS();
		std::vector<RuleInfo::Item*> * rhs = rule->getRHS();
		json = JSONforRule(lhs);
		fs << "'lhs':[\n";
		fs << json->c_str();
		fs << "\n],\n";
		delete json;
		json = JSONforRule(rhs);
		fs << "'rhs':[\n";
		fs << json->c_str();
		fs << "\n]\n";
		fs << "}";
		delete json;
	}
	
	fs << "\n]\n}\n";
	fs << "window['KMKL'] = keyboard;";
	
	fs.close();
	
	delete[] kbTitle;
	if (hotkey) delete[] hotkey;
	
	return true;
}