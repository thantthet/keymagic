#ifndef KEYMAGICENGINE_H_
#define KEYMAGICENGINE_H_

#include <map>

#include "KeyMagicTypes.h"
#include "KeyMagicString.h"
#include "RuleInfo.h"
#include "KeyMagicKeyboard.h"

typedef std::vector<KeyMagicString> TypeContextHistory;

/**
 * IME-engine which processes matching and replacing patterns
 */
class KeyMagicEngine {
public:
	enum {
		SHIFT_MASK = 1 << 0,
		CTRL_MASK = 1 << 1,
		ALT_MASK = 1 << 2,
		CAPS_MASK = 1 << 3
	};
	/**
	 * Load compiled keyboard file
	 * @filename File name of the file to load
	 */
	bool loadKeyboardFile(const char * filename);

	/**
	 * process input key event
	 * @param keyval character value
	 * @param keycode virtual-key code
	 * @param modifier modifier key states (CTRL, SHIFT, ALT)
	 * @return TRUE if key is processed otherwise false
	 */
	bool processKeyEvent(int keyval, int keycode, int modifier);
	/**
	 *
	 */
	void reset() {
		m_textContext.clear();
		for (unsigned int i = 0; i < 256; i++) {
			m_keyStates[i] = 0;
		}
		m_switch.clear();
		m_contextHistory.clear();
	}
	/**
	 *
	 */
	KeyMagicString * getContextText() {
		return &m_textContext;
	}
private:
	int matchKeyStates(int keycode, int modifier, std::vector<RuleInfo::Item*> * rule);
	/**
	 * private function to match input rule
	 * @param ruleInfo rule to match with
	 * @param keyval character value
	 * @param keycode virtual-key code
	 * @param modifier modifier key states (CTRL, SHIFT, ALT)
	 */
	bool matchRule(RuleInfo * rule, int keyval, int keycode, int modifier);
	/**
	 * private function to process output once the rule was matched
	 * @param ruleInfo rule that was matched
	 */
	bool processOutput(RuleInfo *);
	/**
	 * Get the key state of `keycode`
	 * @param keycode keycode to get
	 */
	int getKeyState(int keycode) {
		return m_keyStates[keycode];
	}
	/**
	 * update context history
	 */
	void updateHistory(KeyMagicString text);

	KeyMagicString m_textContext;

	int m_keyStates[256];

	TypeContextHistory m_contextHistory;

	KeyMagicKeyboard m_keyboard;

	std::map<int, bool> m_switch;

	StringList * m_strings;

	RuleList * m_rules;

	LayoutOptions * m_layoutOptions;

	std::vector<KeyMagicString> m_backRef;

	bool matchedVK;
};

#endif
