/*
 * KeyMagicTypes.h
 *
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

#ifndef KEYMAGICTYPES_H_
#define KEYMAGICTYPES_H_

#if defined (_WIN32) || defined (_WIN64)
#include <windows.h>
#endif

#include <vector>
#include <string.h>
#include <map>
#include "KeyMagicString.h"

namespace libkm {

using namespace std;
	/**
	 * BinaryRule is the array of bytes about rule patterns\n
	 * @see ruleInfo::toRuleInfo() to know how these byte can be decoded
	 */
	class BinaryRule {
	private:
		/**
		 * array of binary opcodes for input rule
		 */
		unsigned short * strInRule;
		/**
		 * array of binary opcodes for output rule
		 */
		unsigned short * strOutRule;
	public:
		BinaryRule () {
			strInRule = 0;
			strOutRule = 0;
		}

		BinaryRule (const BinaryRule& rule) {
			strInRule = 0;
			strOutRule = 0;

			SetInRule(rule.strInRule);
			SetOutRule(rule.strOutRule);
		}

		BinaryRule (unsigned short * in_r, unsigned short * out_r) {
			SetInRule(in_r);
			SetOutRule(out_r);
		}

		const unsigned short * GetInRule() {
			return strInRule;
		}

		const unsigned short * GetOutRule() {
			return strOutRule;
		}

		void SetInRule(const unsigned short * data) {
			if (strInRule != 0) {
				delete[] strInRule;
				strInRule = 0;
			}

			int length = GetLength(data);

			strInRule = new unsigned short [length + 1];

			copy(strInRule, data);
			strInRule[length] = 0;
		}

		void SetOutRule(const unsigned short * data) {
			if (strOutRule != 0) {
				delete[] strOutRule;
				strOutRule = 0;
			}

			int length = GetLength(data);

			strOutRule = new unsigned short [length+ 1];

			copy(strOutRule, data);
			strOutRule[length] = 0;
		}

		void copy(unsigned short * dest, const unsigned short * src) {
			int count = GetLength(src);

			memcpy(dest, src, count * sizeof(unsigned short));
		}

		int GetLength(const unsigned short * data) {
			int count = 0;
			while (*data++) {
				count ++;
			}

			return count;
		}

		~BinaryRule () {
			if (strInRule != 0) {
				delete[] strInRule;
			}

			if (strOutRule != 0) {
				delete[] strOutRule;
			}
		}
	} ;

	typedef vector<BinaryRule> BinaryRuleList;
	typedef vector<const unsigned short * > BinaryStringList;
	typedef vector<KeyMagicString> StringList;

	class Info {
	private:
		short size;
		char * data;
	public:
		Info ();
		Info (const Info& info);
		void SetData(char * d, unsigned short s);
		unsigned short Size ();
		const char * Data ();
		~Info ();
	};
	typedef map<int, Info> InfoList;
		
	/**
	 * LayoutOptions use to alternate the behavior of the keyboard layout
	 */
	typedef struct
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
		/**
		 * treat CTRL+ALT as RALT
		 */
		bool rightAlt;
	} LayoutOptions;

	/**
	 * LayoutOptions_1_3 use to alternate the behavior of the keyboard layout
	 */
	typedef struct
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
	} LayoutOptions_1_3;
		
	/**
	 * FileHeader_1_3 is the header structure of compiled keyboard files
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
		LayoutOptions_1_3 layoutOptions;
	} FileHeader_1_3;

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
		LayoutOptions_1_3 layoutOptions; // same with 1.3's layout options
	} FileHeader_1_4;
	
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

}

#endif /* KEYMAGICTYPES_H_ */
