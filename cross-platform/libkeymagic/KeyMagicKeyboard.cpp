/*
 * Copyright (C) 2008  KeyMagic Project
 * http://keymagic.googlecode.com
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <algorithm>

#include "KeyMagicKeyboard.h"

namespace libkm {

	KeyMagicKeyboard::KeyMagicKeyboard() :
		m_logger(KeyMagicLogger::getInstance()),
		m_verbose(0)
	{
	}

	KeyMagicKeyboard::~KeyMagicKeyboard() {
		deleteRules();
		m_infos.clear();
	}

	void KeyMagicKeyboard::deleteRules() {
		/*for (RuleList::iterator i = m_rules.begin(); i != m_rules.end(); i++) {
			delete *i;
		}*/
		m_rules.clear();
	}

	#if defined (_WIN32) || defined (_WIN64)

	//TODO: use ICU converter instead of WideCharToMultiByte to be availabled on all platforms

	DWORD KeyMagicKeyboard::getVersion(const WCHAR * file) {
		char szFile[MAX_PATH];
		int len = WideCharToMultiByte(CP_ACP, 0, file, wcslen(file), szFile, MAX_PATH, 0, 0);
		if (len) {
			szFile[len] = '\0';
			return getVersion(szFile);
		}
		return 0;
	}

	bool KeyMagicKeyboard::loadKeyboardFile(const WCHAR * wcPath) {
		char szPath[MAX_PATH];
		int len = WideCharToMultiByte(CP_UTF8, 0, wcPath, wcslen(wcPath), szPath, MAX_PATH, 0, 0);
		if (len) {
			szPath[len] = '\0';
			return loadKeyboardFile(szPath);
		}
		return false;
	}

	InfoList * KeyMagicKeyboard::getInfosFromKeyboardFile(const WCHAR * file) {
		char szFile[MAX_PATH];
		int len = WideCharToMultiByte(CP_ACP, 0, file, wcslen(file), szFile, MAX_PATH, 0, 0);
		if (len) {
			szFile[len] = '\0';
			return getInfosFromKeyboardFile(szFile);
		}
		return NULL;
	}

	#endif
	unsigned long KeyMagicKeyboard::getVersion(const char * file) {
		FileHeader fh;
		FILE * hFile;

		hFile = fopen(file, "rb");
		if (!hFile){
			PERROR("Cannot open keyboard file : %s\n", file);
			return 0;
		}

		if (ReadHeader(hFile, &fh) == false) {
			PERROR("Fail to load keyboard file : %s\n", file);
			fclose(hFile);
			return 0;
		}

		fclose(hFile);

		return MAKELONG(fh.majorVersion, fh.minorVersion);
	}

	InfoList * KeyMagicKeyboard::getInfosFromKeyboardFile(const char * file) {
		FileHeader fh;
		FILE * hFile;

		InfoList * infos = new InfoList();

		hFile = fopen(file, "rb");
		if (!hFile){
			PERROR("Cannot open keyboard file : %s\n", file);
			return NULL;
		}

		if (ReadHeader(hFile, &fh) == false) {
			PERROR("Fail to load keyboard file : %s\n", file);
			fclose(hFile);
			return NULL;
		}

		for (int i = 0; i < fh.stringCount; i++)
		{
			short sLength;
			fread(&sLength, sizeof(short), 1, hFile);

			short * local_buf = new short[sLength+1];
			local_buf[sLength]='\0';
			fread(local_buf, sLength * sizeof(short), 1, hFile);

			delete[] local_buf;
		}

		for (int i = 0; i < fh.infoCount; i++)
		{
			int id;
			short sLength;

			fread(&id, sizeof(int), 1, hFile);
			fread(&sLength, sizeof(short), 1, hFile);

			char * local_buf = new char[sLength+1];
			local_buf[sLength]='\0';
			fread(local_buf, sLength * sizeof(char), 1, hFile);

			//infos[id].size = sLength;
			//infos[id].data = local_buf;
			infos->operator [](id).SetData(local_buf, sLength);
		}
		
		fclose(hFile);

		return infos;
	}

	bool KeyMagicKeyboard::ReadHeader(FILE * hFile, FileHeader * fh) {
		
		fread(fh, sizeof(FileHeader), 1, hFile);

		if (fh->magicCode[0] != 'K' || fh->magicCode[1] != 'M' || fh->magicCode[2] != 'K' || fh->magicCode[3] != 'L') {
			PERROR("Not KeyMagic keyboard file.\n");
			return false;
		}

		if (fh->majorVersion > MAJOR_VERSION) {
			PERROR("Can't load newer version of keyboard file.\n");
			return false;
		}

		if (fh->minorVersion > MINOR_VERSION) {
			PERROR("Can't load newer version of keyboard file.\n");
			return false;
		}

		if (fh->majorVersion == MAJOR_VERSION && fh->minorVersion == MINOR_VERSION) {
			return true;
		}

		// if older version 1.4
		if (fh->majorVersion == 1) {
			if (fh->minorVersion == 4) {
				FileHeader_1_4 fh14;
				fseek(hFile, 0, SEEK_SET);
				// read the header back
				fread(&fh14, sizeof(FileHeader_1_4), 1, hFile);
				// backward compability
				fh->stringCount = fh14.stringCount;
				fh->infoCount = fh14.infoCount;
				fh->ruleCount = fh14.ruleCount;
				
				fh->layoutOptions.trackCaps = fh14.layoutOptions.trackCaps;
				fh->layoutOptions.autoBksp = fh14.layoutOptions.autoBksp;
				fh->layoutOptions.eat = fh14.layoutOptions.eat;
				fh->layoutOptions.posBased = fh14.layoutOptions.posBased;
				fh->layoutOptions.rightAlt = true; // 1.4 dont have this option
			}
			else if (fh->minorVersion == 3) {
				FileHeader_1_3 fh13;
				fseek(hFile, 0, SEEK_SET);
				// read the header back
				fread(&fh13, sizeof(FileHeader_1_3), 1, hFile);
				// backward compability
				fh->stringCount = fh13.stringCount;
				fh->ruleCount = fh13.ruleCount;
				
				fh->layoutOptions.trackCaps = fh13.layoutOptions.trackCaps;
				fh->layoutOptions.autoBksp = fh13.layoutOptions.autoBksp;
				fh->layoutOptions.eat = fh13.layoutOptions.eat;
				fh->layoutOptions.posBased = fh13.layoutOptions.posBased;
				fh->layoutOptions.rightAlt = true; // 1.3 dont have this option
				
				fh->infoCount = 0; // 1.3 dont have infos, so set it to 0
			}
		}

		return true;
	}

	bool KeyMagicKeyboard::loadKeyboardFromFileDescriptor(int fd) {
		FILE *hFile = fdopen(fd, "rb");

		return loadKeyboardFromFileHandle(hFile);
	}

	bool KeyMagicKeyboard::loadKeyboardFile(const char * szPath) {
		FILE *hFile = fopen(szPath, "rb");

		return loadKeyboardFromFileHandle(hFile);
	}

	bool KeyMagicKeyboard::loadKeyboardFromFileHandle(FILE *hFile) {
		FileHeader fh;

		BinaryRuleList rules;
		BinaryStringList strings;

		if (!hFile){
			PERROR("Cannot open keyboard file");
			return false;
		}

		if (ReadHeader(hFile, &fh) == false) {
			PERROR("Failed to load keyboard file");
			fclose(hFile);
			return false;
		}

		m_layoutOptions = fh.layoutOptions;
		
		if (m_verbose) {
			m_logger->log("autoBksp=%x\n", m_layoutOptions.autoBksp);
			m_logger->log("eat=%x\n", m_layoutOptions.eat);
			m_logger->log("posBased=%x\n", m_layoutOptions.posBased);
			m_logger->log("trackCaps=%x\n", m_layoutOptions.trackCaps);
		}
		
		m_strings.clear();
		deleteRules();
		m_infos.clear();

		for (int i = 0; i < fh.stringCount; i++)
		{
			short sLength;
			fread(&sLength, sizeof(short), 1, hFile);

			unsigned short * local_buf = new unsigned short[sLength+1];
			local_buf[sLength]='\0';
			fread(local_buf, sLength * sizeof(unsigned short), 1, hFile);

			strings.push_back(local_buf);
		}

		for (int i = 0; i < fh.infoCount; i++)
		{
			int id;
			short sLength;

			fread(&id, sizeof(int), 1, hFile);
			fread(&sLength, sizeof(short), 1, hFile);

			char * local_buf = new char[sLength + 1];
			local_buf[sLength]='\0';
			fread(local_buf, sLength * sizeof(char), 1, hFile);

			m_infos[id].SetData(local_buf, sLength);
			
			delete[] local_buf;
		}

		for (int i = 0; i < fh.ruleCount; i++)
		{
			BinaryRule Rule;
			short sLength;

			fread(&sLength, sizeof(short), 1, hFile);
			unsigned short * ruleBinaryIn = new unsigned short[sLength+1];
			ruleBinaryIn[sLength]='\0';
			fread(ruleBinaryIn, sLength * sizeof(short), 1, hFile);

			fread(&sLength, sizeof(short), 1, hFile);
			unsigned short * ruleBinaryOut = new unsigned short[sLength+1];
			ruleBinaryOut[sLength]='\0';
			fread(ruleBinaryOut, sLength * sizeof(short), 1, hFile);

			Rule.SetInRule(ruleBinaryIn);
			Rule.SetOutRule(ruleBinaryOut);

			rules.push_back(Rule);
			
			delete[] ruleBinaryIn;
			delete[] ruleBinaryOut;
		}

		//Convert variable to pure string. Originally, variables are binary arrays containing inline index
		variablesToStrings(&strings, &m_strings);
		binaryRulesToManagedRules(&rules, &m_rules);
		std::sort(m_rules.begin(), m_rules.end());
		std::reverse(m_rules.begin(), m_rules.end());
		
		if (m_verbose) {
			m_logger->log("strings;%d==%d\n", strings.size(), m_strings.size());
			m_logger->log("rules;%d==%d\n", rules.size(), m_rules.size());
			m_logger->log("|----rules-start----|\n");
			for (RuleList::iterator i = m_rules.begin(); i != m_rules.end(); i++) {
				RuleInfo rule = *i;
				std::string * s;
				
				if (i != m_rules.begin()) {
					m_logger->log(",\n");
				}
				
				m_logger->log("|---index=%d---|\n{\n", rule.getRuleIndex());
				s = rule.description();
				m_logger->log(s->c_str());
				m_logger->log("}");
				delete s;
			}
			m_logger->log("\n|----rules-end----|\n");
		}
		
		fclose(hFile);
		
		for (BinaryStringList::iterator i = strings.begin(); i != strings.end(); i++) {
			delete[] *i;
		}
        
		rules.clear();

		return true;
	}

	StringList * KeyMagicKeyboard::getStrings() {
		return &m_strings;
	}

	RuleList * KeyMagicKeyboard::getRules() {
		return &m_rules;
	}

	LayoutOptions * KeyMagicKeyboard::getLayoutOptions() {
		return &m_layoutOptions;
	}

	const InfoList& KeyMagicKeyboard::getInfoList() {
		return m_infos;
	}

	void KeyMagicKeyboard::variablesToStrings(BinaryStringList * variables, StringList * strings) {

		for (BinaryStringList::iterator i = variables->begin(); i != variables->end(); i++) {

			const unsigned short * binString = *i;
			KeyMagicString value;

			while(*binString) {
				switch(*binString) {
				case opVARIABLE:
					binString++;
					value += getVariableValue(*binString++, variables);
					break;
				default:
					value += *binString++;
				}
			}
			strings->push_back(value);
		}
	}

	void KeyMagicKeyboard::binaryRulesToManagedRules(BinaryRuleList * binRules, RuleList * rules) {
		for (BinaryRuleList::iterator i = binRules->begin(); i != binRules->end(); i++) {
			RuleInfo * managedMatchRule = new RuleInfo(i->GetInRule(), i->GetOutRule(), &m_strings);
			managedMatchRule->setIndex(i - binRules->begin());
			rules->push_back(*managedMatchRule);
			delete managedMatchRule;
		}
	}

	KeyMagicString KeyMagicKeyboard::getVariableValue(int index, BinaryStringList * binStrings) {
		KeyMagicString value;

		index--;

		if (index > binStrings->size()) {
			return KeyMagicString();
		}

		const unsigned short * binRule = binStrings->at(index);

		while (*binRule) {
			if (*binRule == opVARIABLE) {
				binRule++;
				KeyMagicString value2 = getVariableValue(*binRule++, binStrings);
				value += value2;
			} else {
				value += *binRule++;
			}
		}

		return value;
	}

}