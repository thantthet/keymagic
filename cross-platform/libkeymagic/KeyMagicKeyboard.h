#ifndef KEYMAGICKEYBOARD_H_
#define KEYMAGICKEYBOARD_H_

#define MAJOR_VERSION 1
#define MINOR_VERSION 4

#include "KeyMagicTypes.h"
#include "KeyMagicString.h"
#include "RuleInfo.h"
#include "KeyMagicLogger.h"

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
	KeyMagicKeyboard();
	/**
	 *
	 */
	~KeyMagicKeyboard();
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
	/**
	 * Get infos
	 */
	const InfoList& getInfoList();
	
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
	void deleteInfos();
};

#endif
