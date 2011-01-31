#ifndef RULEINFO_H_
#define RULEINFO_H_

#include <string>
#include <vector>

#include "KeyMagicString.h"
#include "KeyMagicTypes.h"

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

/**
 * ruleInfo used for LHS and RHS rules
 */

class RuleInfo {
public:
	/**
	 * Types of item
	 * @see item
	 */
	enum types {
		tString,
		tAnyOfString,
		tNotOfString,
		tBackRefString,
		tReference,
		tVKey,
		tAny,
		tSwitch
	};

	/**
	 * item structure store all information needed to match\n
	 * each item represent a segment of a pattern eg. (in script) seg1 + seg2 => seg3 + seg4\n
	 * seg1 and seg2 are items, two items\n
	 */
	struct Item {
		types type;
		int switchId;
		int refIndex;
		int keyCode;
		KeyMagicString * stringValue;

		Item(types t, const KeyMagicString * string) {
			stringValue = new KeyMagicString();
			stringValue->assign(string->c_str());
			type = t;
		}

		Item(types t, int i) {
			if (t == tReference){
				refIndex = i;
				type = t;
			} else if (t == tVKey) {
				keyCode = i;
				type = t;
			} else if (t == tSwitch) {
				switchId = i;
				type = t;
			}
		}

		Item (types t) {
			type = t;
		}
	};

	/**
	 * @param in sequence of binary rule for input
	 * @param out sequence of binary rule for output
	 * @param vars strings list of variables
	 */
	RuleInfo(short * in, short * out, StringList * vars);

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
	 *
	 */
	unsigned int getSwitchCount() {
		return m_countSwitch;
	}
	/**
	 *
	 */
	unsigned int getVKCount() {
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
	 * @param i index of rule
	 */
	void setIndex(int index) {
		m_index = index;
	}
private:
	int m_index;
	/**
	 * context length to be matched
	 */
	unsigned int m_matchLength;
	/**
	 * list of switches' id that we need to match
	 */
	std::vector<int> m_switches;
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
};

typedef std::vector<RuleInfo*> RuleList;

#endif /* RULEINFO_H_ */
