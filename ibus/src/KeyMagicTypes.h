/*
 * KeyMagicTypes.h
 *
 *  Created on: Jan 28, 2011
 *      Author: thantthetkz
 */

#ifndef KEYMAGICTYPES_H_
#define KEYMAGICTYPES_H_

#include <vector>
#include "KeyMagicString.h"

/**
 * BinaryRule is the array of bytes about rule patterns\n
 * @see ruleInfo::toRuleInfo() to know how these byte can be decoded
 */
typedef struct _BinaryRule {
	short* strInRule;
	short* strOutRule;
} BinaryRule;

typedef std::vector<BinaryRule> BinaryRuleList;
typedef std::vector<const short * > BinaryStringList;
typedef std::vector<KeyMagicString> StringList;
/**
 * LayoutOptions use to alternate the behavior of the keyboard layout\n
 * trackCaps - consider CAPSLOCK key in matching\n
 * autoBksp - smart backspace\n
 * eat - eat the key if there is no match\n
 * posBased - layout is based on the US keyboard Layout\n
 */
typedef struct _LayoutOptions
{
	bool trackCaps;
	bool autoBksp;
	bool eat;
	bool posBased;
} LayoutOptions;
/**
 * FileHeader is the header structure of compiled keyboard files\n
 * magicCode - always 'KMKL'\n
 * majorVersion - KeyMagic version to use with\n
 * minorVersion - KeyMagic version to use with\n
 * stringCount - Count of strings/variables\n
 * ruleCount - Count of rules\n
 */
typedef struct _FileHeader
{
	char magicCode[4];
	char majorVersion;
	char minorVersion;
	short stringCount;
	short ruleCount;
	LayoutOptions layoutOptions;
} FileHeader;


#endif /* KEYMAGICTYPES_H_ */
