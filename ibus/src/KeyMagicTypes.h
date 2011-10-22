/*
 * KeyMagicTypes.h
 *
 *  Created on: Jan 28, 2011
 *      Author: thantthetkz
 */

#ifndef KEYMAGICTYPES_H_
#define KEYMAGICTYPES_H_

#if defined (_WIN32) || defined (_WIN64)
#include <windows.h>
#endif

#include <vector>
#include <map>
#include "KeyMagicString.h"

using namespace std;
/**
 * BinaryRule is the array of bytes about rule patterns\n
 * @see ruleInfo::toRuleInfo() to know how these byte can be decoded
 */
typedef struct {
	/**
	 * array of binary opcodes for input rule
	 */
	short* strInRule;
	/**
	 * array of binary opcodes for output rule
	 */
	short* strOutRule;
} BinaryRule;

typedef vector<BinaryRule> BinaryRuleList;
typedef vector<const short * > BinaryStringList;
typedef vector<KeyMagicString> StringList;

typedef struct {
	short size;
	char * data;
} Info;
typedef map<int, Info> InfoList;

/**
 * LayoutOptions use to alternate the behavior of the keyboard layout
 */
typedef struct LayoutOptions
{
	/**
	 * consider CAPSLOCK key in matching
	 */
	bool trackCaps;
	/**
	 * smart backspace
	 */
	bool autoBksp;
	/**
	 * eat the key if there is no match
	 */
	bool eat;
	/**
	 * layout is based on the US keyboard Layout
	 */
	bool posBased;
} LayoutOptions;
/**
 * FileHeader is the header structure of compiled keyboard files
 */
typedef struct {
	/**
	 * always 'KMKL'
	 */
	char magicCode[4];
	/**
	 * KeyMagic version to use with
	 */
	char majorVersion;
	/**
	 * KeyMagic version to use with
	 */
	char minorVersion;
	/**
	 * Count of strings/variables
	 */
	short stringCount;
	/**
	 * Count of rules
	 */
	short ruleCount;
	/**
	 * Layout Options of this file
	 */
	LayoutOptions layoutOptions;
} FileHeader_1_3;

typedef struct {
	/**
	 * always 'KMKL'
	 */
	char magicCode[4];
	/**
	 * KeyMagic version to use with
	 */
	char majorVersion;
	/**
	 * KeyMagic version to use with
	 */
	char minorVersion;
	/**
	 * Count of strings/variables
	 */
	short stringCount;
	/**
	 * Cont of infos
	 */
	short infoCount;
	/**
	 * Count of rules
	 */
	short ruleCount;
	/**
	 * Layout Options of this file
	 */
	LayoutOptions layoutOptions;
} FileHeader;

#endif /* KEYMAGICTYPES_H_ */
