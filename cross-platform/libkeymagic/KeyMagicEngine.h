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

#ifndef KEYMAGICENGINE_H_
#define KEYMAGICENGINE_H_

#include <map>

#include "KeyMagicTypes.h"
#include "KeyMagicString.h"
#include "RuleInfo.h"
#include "KeyMagicKeyboard.h"
#include "KeyMagicLogger.h"

#define KeyStateLen 256

namespace libkm {

typedef std::vector<KeyMagicString> TContextHistory;

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

	bool loadKeyboardFromFileDescriptor(int fd);

	bool loadKeyboardFromFileHandle(FILE *hFile);
	
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
	KeyMagicString getContextText();
	/**
	 * Set current context
	 * @param textContext string to set as context
	 */
	void setContextText(KeyMagicString const &textContext);
	/**
	 * Get difference between engine's context and given context
	 * @param contextBefore string to compare againt engine's context
	 * @param difference result string
	 * @return count of delete that should be send before sending result conetxt
	 */
	unsigned long getDifference(KeyMagicString const &contextBefore, KeyMagicString *difference);
	/**
	 * Get keyboard object
	 */
	KeyMagicKeyboard * getKeyboard();

	/**
	 * Get the key state of `keycode`
	 * @param keycode keycode to get
	 */
	short getKeyState(int keycode);
	/**
	 * Set keystate
	 * @param keycode keycode to set
	 * @param state state of keycode
	 */
	void setKeyState(int keycode, unsigned char state);
	/**
	 * Set all key states
	 * @param keystates array of keystate
	 */
	void setKeyStates(unsigned char * keystates);

	/**
	 * Get switch map 
	 * @return current switch map
	 */
	std::map<int, bool> getSwitchMap();

	/**
	 * Set switch map 
	 * @param switchMap switch map to set
	 */
	void setSwitchMap(const std::map<int, bool> &switchMap);
	
	/**
	 * Get history
	 * @return current TContextHistory
	 */
	TContextHistory getHistory();
	
	/**
	 * Set history
	 * @param history TContextHistory to set
	 */
	void setHistory(const TContextHistory &history);
	
	bool m_verbose;
private:
	int matchKeyStates(int modifier, RuleInfo::ItemList* rule);
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
	void prepareForMatching();
	/**
	 * update context history
	 */
	void updateHistory(const KeyMagicString &text);
	
	KeyMagicString m_textContext;
	
	unsigned char m_keyStates[KeyStateLen];
	
	TContextHistory m_contextHistory;
	
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
	unsigned int m_indent;
};

}

#endif
