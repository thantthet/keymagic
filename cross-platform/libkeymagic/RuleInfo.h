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

#ifndef RULEINFO_H_
#define RULEINFO_H_

#include <string>
#include <vector>

#include "KeyMagicString.h"
#include "KeyMagicTypes.h"
#include "KeyMagicLogger.h"

namespace libkm {

/*
#define opSTRING		0x00F0
#define opVARIABLE		0x00F1
#define opREFERENCE		0x00F2
#define opPREDEFINED	0x00F3
#define opMODIFIER		0x00F4
#define opANYOF			0x00F5
#define opAND			0x00F6
#define opNANYOF		0x00F7
#define opANY			0x00F8
#define opSWITCH		0x00F9
 */
enum {
	opSTRING = 0x00f0,
	opVARIABLE = 0x00f1,
	opREFERENCE = 0x00f2,
	opPREDEFINED = 0x00f3,
	opMODIFIER = 0x00f4,
	opANYOF = 0x00f5,
	opAND = 0x00f6,
	opNANYOF = 0x00f7,
	opANY = 0x00f8,
	opSWITCH = 0x00f9
};

/**
 * ruleInfo used for LHS and RHS rules
 */

class RuleInfo {
public:
	/**
	 * Types of item
	 * @see Item
	 */
	enum types {
		/**
		 * Normal String
		 */
		tString,
		/**
		 * Any one character of the string to be matched
		 */
		tAnyOfString,
		/**
		 * No character in string should be matched
		 */
		tNotOfString,
		/**
		 * Get matched index of tAnyOfString form the LHS and replace with character at index of this string
		 */
		tBackRefString,
		/**
		 * Get matched value from LHS and replace
		 */
		tReference,
		/**
		 * Virtual Key Code
		 */
		tVKey,
		/**
		 * Any character
		 */
		tAny,
		/**
		 * Switch
		 */
		tSwitch
	};

	/**
	 * item structure stores all details needed to be matched\n
	 * each item represent a segment of a pattern eg. (which is in script) seg1 + seg2 => seg3 + seg4\n
	 * seg1, seg2 and seg(n) each are item\n
	 */
	struct Item {
		/**
		 * Type of item
		 */
		types type;
		/**
		 * Switch id when type is tSwitch
		 */
		int switchId;
		/**
		 * Reference index when type is tReference or tBackRefString
		 */
		int refIndex;
		/**
		 * Keycode value when type is tVKey
		 */
		int keyCode;
		/**
		 * String value when type is tString, tBackRefString, tAnyOfString, tNotAnyOfString
		 */
		KeyMagicString * stringValue;
		
		/**
		 * construct new Item for type tString, tBackRefString, tAnyOfString, tNotAnyOfString
		 * @param t type of this Item
		 * @param string string value of this Item
		 */
		Item(types t, const KeyMagicString * string);
		/**
		 * construct new Item for tSwitch, tVKKey, tReference
		 * @param t type of this Item
		 * @param i Could be switchId or Reference Index or VK code
		 */
		Item(types t, int i);
		/**
		 * construct new Item for tAny
		 * @param t type of this Item
		 */
		Item (types t);
		/**
		 * deconstructor
		 */
		~Item();
	};

	/**
	 * @param in sequence of binary rule for input
	 * @param out sequence of binary rule for output
	 * @param vars strings list of variables
	 */
	RuleInfo(short * in, short * out, StringList * vars);
	/**
	 *
	 */
	~RuleInfo();
	/**
	 * @param binRule binary rule to convert
	 * @param outRule output (result) managed rule
	 * @param variable list of variables string for lookup
	 */
	int toRuleInfo(short * binRule, std::vector<Item*> * outRule, StringList * variable);

	/**
	 * @return list of LHS rules
	 */
	std::vector<Item*> * getLHS() {
		return &m_lhsRule;
	}
	/**
	 * @return list of RHS rules
	 */
	std::vector<Item*> * getRHS() {
		return &m_rhsRule;
	}
	/**
	 * @return length of string to match (no keys, no switches)
	 */
	unsigned int getMatchLength() {
		return m_matchLength;
	}
	/**
	 * Get switch count of lhs rule
	 * @return switch count of lhs rule
	 */
	unsigned int getLHSSwitchCount() {
		return m_countSwitch;
	}
	/**
	 * Get vk count of lhs rule
	 * @return vk count of lhs rule
	 */
	unsigned int getLHSVKCount() {
		return m_countVkey;
	}
	/**
	 * @return orginal index of rule
	 */
	int getRuleIndex() {
		return m_index;
	}
	/**
	 * Set the index of rule
	 * @param index index of rule
	 */
	void setIndex(int index) {
		m_index = index;
	}
	/**
	 * Get human-switchId string representing of this rule
	 * @return string of rule
	 */
	std::string * description();

private:
	std::string * JSON(std::vector<Item*> * rule);
	int getSwitchCount(std::vector<Item*> * rule);
	int getVKCount(std::vector<Item*> * rule);
	
	int m_index;
	/**
	 * context length to be matched
	 */
	unsigned int m_matchLength;
	/**
	 * array of left-hand-side rules
	 */
	std::vector<Item*> m_rhsRule;
	/**
	 * array of right-hand-side rules
	 */
	std::vector<Item*> m_lhsRule;
	/**
	 * LHS switch count
	 */
	unsigned int m_countSwitch;
	/**
	 * LHS vk count
	 */
	unsigned int m_countVkey;
	KeyMagicLogger * m_logger;
};

typedef std::vector<RuleInfo*> RuleList;

}

#endif /* RULEINFO_H_ */

