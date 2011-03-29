#ifndef KEYMAGICENGINE_H_
#define KEYMAGICENGINE_H_

#include <map>

#include "KeyMagicTypes.h"
#include "KeyMagicString.h"
#include "RuleInfo.h"
#include "KeyMagicKeyboard.h"
#include "KeyMagicLogger.h"

/* The classes below are exported */
#define KeyStateLen 256

typedef std::vector<KeyMagicString> TypeContextHistory;

/**
 * IME-engine which processes matching and replacing patterns
 */
class KeyMagicEngine  {
public:
	KeyMagicEngine();
	~KeyMagicEngine();
	
	enum {
		SHIFT_MASK = 1 << 0,
		CTRL_MASK = 1 << 1,
		ALT_MASK = 1 << 2,
		CAPS_MASK = 1 << 3
	};
	/**
	 * Load compiled keyboard file
	 * @param filename File name of the file to load
	 */
	bool loadKeyboardFile(const char * filename);
#if defined (_WIN32) || defined (_WIN64)
	bool loadKeyboardFile(const WCHAR * filename);
#endif
	/**
	 * process input key event
	 * @param keyval character value
	 * @param keycode virtual-key code
	 * @param modifier modifier key states (CTRL, SHIFT, ALT)
	 * @return TRUE if key is processed otherwise false
	 */
	bool processKeyEvent(int keyval, int keycode, int modifier);
	/**
	 * reset the context, switch, keystates
	 */
	void reset();
	/**
	 * Get current context
	 */
	KeyMagicString * getContextText();
	/**
	 * Set current context
	 */
	void setContextText(KeyMagicString * textContext);
	/**
	 * Get keyboard object
	 */
	KeyMagicKeyboard * getKeyboard();
	
	bool m_verbose;
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
	 *
	 */
	bool processInput(int keyval, int keycode, int modifier);
	/**
	 * private function to process output once the rule was matched
	 * @param ruleInfo rule that was matched
	 */
	bool processOutput(RuleInfo *);
	/**
	 * Get the key state of `keycode`
	 * @param keycode keycode to get
	 */
	int getKeyState(int keycode);
	void prepareForMatching();
	/**
	 * update context history
	 */
	void updateHistory(KeyMagicString text);
	
	KeyMagicString m_textContext;
	
	int m_keyStates[KeyStateLen];
	
	TypeContextHistory m_contextHistory;
	
	KeyMagicKeyboard m_keyboard;
	
	std::map<int, bool> m_switch;
	
	StringList * m_strings;
	
	RuleList * m_rules;
	
	LayoutOptions * m_layoutOptions;
	
	std::vector<KeyMagicString> m_backRef;
	
	bool m_matchedVK;
	
	bool m_shouldMatchAgain;
	
	bool m_haveKeyboard;
	
	KeyMagicLogger * m_logger;
};

#endif
