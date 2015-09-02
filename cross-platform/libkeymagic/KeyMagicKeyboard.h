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

#ifndef KEYMAGICKEYBOARD_H_
#define KEYMAGICKEYBOARD_H_

#define MAJOR_VERSION 1
#define MINOR_VERSION 5

#include "KeyMagicTypes.h"
#include "KeyMagicString.h"
#include "RuleInfo.h"
#include "KeyMagicLogger.h"
#include "KeyMagicErrorLogger.h"

#ifndef MAKELONG
#define MAKELONG(a, b)      (a & 0xffff) | ((a & 0xffff) << 16)
#endif

namespace libkm {

#define KML_INFO_NAME 'name'
#define KML_INFO_DESC 'desc'
#define KML_INFO_FONT 'font'
#define KML_INFO_ICON 'icon'
#define KML_INFO_HOTKEY 'htky'
/**
 * Keyboard Data
 */
class KeyMagicKeyboard {
public:
	/**
	 * Constructor
	 */
	KeyMagicKeyboard();
	/**
	 * Deconstructor
	 */
	~KeyMagicKeyboard();
	/**
	 * Load compiled keyboard layout using filename
	 * @param filename File name of keyboard to be loaded
	 */
	bool loadKeyboardFile(const char * filename);

	bool loadKeyboardFromFileDescriptor(int fd);

	bool loadKeyboardFromFileHandle(FILE *hFile);
	
#if defined (_WIN32) || defined (_WIN64)
	bool loadKeyboardFile(const WCHAR * filename);
#endif
	/**
	 * Get the list of strings
	 */
	StringList * getStrings();
	/**
	 * Get the list of managed rules
	 */
	RuleList * getRules();
	/**
	 * Get layout options
	 */
	LayoutOptions * getLayoutOptions();
	/**
	 * Get infos
	 */
	const InfoList& getInfoList();

	static bool ReadHeader(FILE * hFile, FileHeader * fh);

#if defined (_WIN32) || defined (_WIN64)
	static unsigned long getVersion(const WCHAR * file);
	static InfoList * getInfosFromKeyboardFile(const WCHAR * file);
#endif
	static unsigned long getVersion(const char * file);
	static InfoList * getInfosFromKeyboardFile(const char * file);
	
	bool m_verbose;
private:

	StringList m_strings;
	InfoList m_infos;
	RuleList m_rules;
	LayoutOptions m_layoutOptions;
	KeyMagicLogger * m_logger;

	void variablesToStrings(BinaryStringList * variables, StringList * strings);
	void binaryRulesToManagedRules(BinaryRuleList * binRules, RuleList * rules);
	bool convertBinaryRuleToManagedRule(const BinaryRule * binRule, RuleInfo * managedMatchRule);
	KeyMagicString getVariableValue(int index, BinaryStringList * binStrings);
	void deleteRules();
};

}

#endif
