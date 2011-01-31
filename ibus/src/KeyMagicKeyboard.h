#ifndef KEYMAGICKEYBOARD_H_
#define KEYMAGICKEYBOARD_H_

#include "KeyMagicTypes.h"
#include "KeyMagicString.h"
#include "RuleInfo.h"

/**
 * Keyboard Data
 */
class KeyMagicKeyboard {
public:
	/**
	 * Load compiled keyboard layout using filename
	 * @param filename File name of keyboard to be loaded
	 */
	bool loadKeyboardFile(const char * filename);
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
private:

	StringList m_strings;
	RuleList m_rules;
	LayoutOptions m_layoutOptions;

	void variablesToStrings(BinaryStringList * variables, StringList * strings);
	void binaryRulesToManagedRules(BinaryRuleList * binRules, RuleList * rules);
	bool convertBinaryRuleToManagedRule(const BinaryRule * binRule, RuleInfo * managedMatchRule);
	KeyMagicString getVariableValue(int index, BinaryStringList * binStrings);
};

#endif
