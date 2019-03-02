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

#include <iostream>
#include <vector>
#include <stdio.h>

#include "KeyMagicKeyboard.h"
#include "KeyMagicEngine.h"
#include "KeyMagicLogger.h"
#include "Util.h"

namespace libkm {

	KeyMagicEngine::KeyMagicEngine() :
		m_logger(KeyMagicLogger::getInstance()),
		m_verbose(0),
		m_haveKeyboard(0),
		m_indent(0)
	{
	}

	KeyMagicEngine::~KeyMagicEngine() {
	}

	bool KeyMagicEngine::loadKeyboardFile(const char * keyboardFile) {
		bool success;
		
		//load keyboard from file
		success = m_keyboard.loadKeyboardFile(keyboardFile);
		
		//keyboard failed to load ?
		if (!success) {
			m_haveKeyboard = false;
			return false;
		}

		prepareForMatching();
		return true;
	}

#ifdef _POSIX_SOURCE
	bool KeyMagicEngine::loadKeyboardFromFileDescriptor(int fd) {
		bool success;
		
		//load keyboard from file
		success = m_keyboard.loadKeyboardFromFileDescriptor(fd);
		
		//keyboard failed to load ?
		if (!success) {
			m_haveKeyboard = false;
			return false;
		}

		prepareForMatching();
		return true;
	}
#endif

	bool KeyMagicEngine::loadKeyboardFromFileHandle(FILE *hFile) {
		bool success;
		
		//load keyboard from file
		success = m_keyboard.loadKeyboardFromFileHandle(hFile);
		
		//keyboard failed to load ?
		if (!success) {
			m_haveKeyboard = false;
			return false;
		}

		prepareForMatching();
		return true;
	}

	#if defined (_WIN32) || defined (_WIN64)
	bool KeyMagicEngine::loadKeyboardFile(const WCHAR * keyboardFile) {
		bool success;
		
		//load keyboard from file
		success = m_keyboard.loadKeyboardFile(keyboardFile);
		
		//keyboard failed to load ?
		if (!success) {
			m_haveKeyboard = false;
			return false;
		}

		prepareForMatching();
		return true;
	}
	#endif

	void KeyMagicEngine::prepareForMatching() {
		//prepare rules to match
		
		m_strings = m_keyboard.getStrings();
		m_rules = m_keyboard.getRules();
		m_layoutOptions = m_keyboard.getLayoutOptions();
		
		//now we have vaild keyboard in memory
		m_haveKeyboard = true;
	}

	short KeyMagicEngine::getKeyState(int keycode) {
		return m_keyStates[keycode];
	}

	void KeyMagicEngine::setKeyState(int keycode, unsigned char state) {
		m_keyStates[keycode] = state;
	}

	void KeyMagicEngine::setKeyStates(unsigned char * states){
		memcpy(m_keyStates, states, sizeof(unsigned char) * 256);
	}

	void KeyMagicEngine::updateHistory(const KeyMagicString &text) {
		LOG_FUNC();
		std::string str;
		
		str = getCharacterReferenceString(text);
		LOG("updateHistory with text:%s\n", str.c_str());
		
		LOG("m_contextHistory.size before updated = %d\n", m_contextHistory.size());
		if (m_contextHistory.size() == 0) {
			LOG("no preivous history\n");
			m_contextHistory.push_back(text);
			LOG("updated\n");
		} else if (text.compare(m_contextHistory.back()) != 0) {
			str = getCharacterReferenceString(m_contextHistory.back()); 
			LOG("need updating. m_contextHistory.back() before update = %s\n", str.c_str());
			m_contextHistory.push_back(text);
			LOG("updated\n");
		} else {
			LOG("no need updating\n");
		}
	}

	bool KeyMagicEngine::processInput(int keyval, int keycode, int modifier) {
		LOG_FUNC();
		LOG("keycode=%x, modifier=%x, keyval=%c\n", keycode, modifier, keyval);
		
		bool success = false;

		for (RuleList::iterator i = m_rules->begin(); i != m_rules->end(); i++) {
			RuleInfo rule = *i;
			bool v = m_verbose;
			m_verbose = false;
			success = matchRule(&rule, keyval, keycode, modifier);
			m_verbose = v;
			if (success) {
				
				LOG("rule id matched: %d\n", rule.getRuleIndex());
				
				if (keycode) {
					m_switch.clear();
				}
				
				// if anything is matched with string pattern, we need to append keyval to the context
				if (!m_matchedVK && keyval != 0) {
					LOG("m_matchedVK: %d\n", m_matchedVK);
					m_textContext += keyval;
				}
				
				success = processOutput(&rule);
				
				//un-press
				m_keyStates[keycode] = 0;
				LOG("SUCCESS\n");
				return success;
			}
		}
		LOG("FAILED\n");
		return false;
	}

	bool KeyMagicEngine::processKeyEvent(int keyval, int keycode, int modifier) {
		LOG_FUNC();
		LOG("keycode=%x, modifier=%x, keyval=%c\n", keycode, modifier, keyval);
		LOG("current context = %s\n", getCharacterReferenceString(m_textContext).c_str());
		bool success = false;
		
		//if there is no keyboard loaded
		if (m_haveKeyboard == false) {
			return false;
		}

		if (keycode > 255) {
			return false;
		}
		
		m_keyStates[keycode] = 0x80;
		
		KeyMagicString oldText = m_textContext;
		
		if (m_keyboard.getLayoutOptions()->trackCaps == true) {
			//TODO: handle trackCaps option
		}
		
		success = processInput(keyval, keycode, modifier);
		//un-press
		m_keyStates[keycode] = 0;
		
		//not success, no control key
		if (!success && keycode >= 0x20) {
			LOG("reseting switch since process input failed and keycode > 0x20\n");
			m_switch.clear();
		}

		int looped = 0;
		if (success) {
			while (success) {
				if (m_shouldMatchAgain) {
					LOG("matching again\n");
					success = processInput(0, 0, 0);
					
					looped++;
					if (looped == 20) {
						LOG("Maximum matching loop exceeded.\n");
						return false;
					}
					
					if (keycode == 8 && m_contextHistory.size()) {
						if (m_textContext == m_contextHistory.back()) { // if results are the same
							LOG("Result and history stack are the same");
							m_contextHistory.pop_back(); // sync with smartbackspace
						} else {
							LOG("Result and history stack are the diff");
							m_contextHistory.clear();
						}
					} else if (!success) {
						updateHistory(oldText);
						return true;
					}

				} else if (keycode == 8 && m_contextHistory.size()) {
					if (m_textContext == m_contextHistory.back()) { // if results are the same
						LOG("Result and history stack are the same");
						m_contextHistory.pop_back(); // sync with smartbackspace
					} else {
						LOG("Result and history stack are the diff");
						m_contextHistory.clear();
					}
					return true;
				} else {
					updateHistory(oldText);
					return true;
				}
			}
			
			return true;
			
		} else {
			std::string str = getCharacterReferenceString(m_textContext); 
			
			if ((modifier & CTRL_MASK) || (modifier & ALT_MASK)) {
				return false;
			}
			
			if (keycode == 8) {
				LOG("autoBksp = %d, m_contextHistory.size = %d\n", m_layoutOptions->autoBksp, m_contextHistory.size());
				if (m_layoutOptions->autoBksp == true && m_contextHistory.size() != 0) {
					m_textContext = m_contextHistory.back();
					m_contextHistory.pop_back();
				} else if (m_textContext.length()) {
					m_textContext = m_textContext.substr(0, m_textContext.length() - 1);
				} else {
					return false;
				}
				return true;
			}
			
			//no match found
			//if not char key, dont eat
			if (keycode <= 0x20 || keyval == 0) {
				return false;
			//but check if `eat` options is active
			} else if (m_layoutOptions->eat == true) {
				//if yes, eat the key and return as processed
				return true;
			} else {
				m_textContext += keyval;
				return true;
			}
		}
		
		return false;
	}

	int KeyMagicEngine::matchKeyStates(int modifier, RuleInfo::ItemList* rules) {
		LOG_FUNC();
		
		int matchedCount = 0;
		int modStates = modifier;
		
		for (RuleInfo::ItemList::iterator i = rules->begin(); i != rules->end(); i++) {
			RuleInfo::Item * curRule = &*i;
			if (curRule->type == RuleInfo::tVKey) {
				switch (curRule->keyCode) {
					case VK_SHIFT:
					case VK_RSHIFT:
					case VK_LSHIFT:
						if (modifier & SHIFT_MASK) {
							modStates -= SHIFT_MASK;
						} else {
							return -1;
						}
						break;
					case VK_CONTROL:
					case VK_RCONTROL:
					case VK_LCONTROL:
						if (modifier & CTRL_MASK) {
							modStates -= CTRL_MASK;
						} else {
							return -1;
						}
						break;
					case VK_MENU:
					case VK_RMENU:
					case VK_LMENU:
						if (modifier & ALT_MASK) {
							modStates -= ALT_MASK;
						} else {
							return -1;
						}
						break;
				}
				if ((m_keyStates[curRule->keyCode] & 0x80) == false) {
					if (curRule->keyCode == VK_RMENU && m_layoutOptions->rightAlt) {
						if ((m_keyStates[VK_MENU] & 0x80) && (m_keyStates[VK_CONTROL] & 0x80)) {
							modStates -= CTRL_MASK;
						}
						else return -1;
					} else {
						return -1;
					}
				}
				matchedCount++;
			}
		}
		
		if ((modStates & CTRL_MASK) || (modStates & ALT_MASK)) {
			return -1;
		}
		
		return matchedCount;
	}

	bool KeyMagicEngine::matchRule(RuleInfo * rule, int keyval, int keycode, int modifier) {
		LOG_FUNC();
		LOG("rule = %d\n", rule->getRuleIndex());
		KeyMagicString appendedContext = m_textContext;
		
		RuleInfo::ItemList * rules = rule->getLHS();
		
		m_matchedVK = false;
		int kcode = matchKeyStates(modifier, rules);
		if (kcode == -1) {
			LOG("Key-states do not matched\n");
			return false;
		} else if (kcode == 0) {
			if (keyval != 0) appendedContext += keyval;
		} else {
			m_matchedVK = true;
		}
		
		unsigned long lenToMatch = rule->getMatchLength();
		unsigned long lenAppended = appendedContext.length();
		
		if (lenToMatch > lenAppended) {
			LOG("Not enough length to match\n");
			return false;
		}
		
		//get from the end
		KeyMagicString stringToMatch = appendedContext.substr(lenAppended - lenToMatch);
		KeyMagicString::iterator itToMatch = stringToMatch.begin();
		
		m_backRef.clear();
		
		for (RuleInfo::ItemList::iterator i = rules->begin(); i != rules->end(); i++) {
			RuleInfo::Item * curRule = &*i, * nextRule;
			KeyMagicString * strPattern;
			//bool found;
			
			RuleInfo::ItemList::iterator ii = i+1;
			if (ii == rules->end()) {
				nextRule = NULL;
			} else {
				nextRule = &*ii;
			}
			
			switch (curRule->type) {
				case RuleInfo::tString:
					LOG("tString\n");
					strPattern = &curRule->stringValue;
					for (KeyMagicString::iterator ii = strPattern->begin(); ii != strPattern->end(); ii++) {
						// if itMatch is at the end
						if (itToMatch == stringToMatch.end()) {
							LOG("--Not matched\n");
							return false;
							// if not match
						} else if (*itToMatch++ != *ii) {
							LOG("--Not matched\n");
							return false;
						}
					}
					LOG("--Matched\n");
					m_backRef.push_back(*strPattern);
					break;
				case RuleInfo::tAnyOfString:
					LOG("tAnyOfString\n");
					strPattern = &curRule->stringValue;

					/*for (KeyMagicString::iterator ii = strPattern->begin(); ii != strPattern->end(); ii++) {
						if (*itToMatch == *ii) {
							found = true;
							break;
						}
						found = false;
					}*/
					// if not found
					if (strPattern->find(*itToMatch) == KeyMagicString::npos) {
						LOG("--Not matched\n");
						return false;
					} else {
						LOG("--Matched \\%x\n", *itToMatch);
						strPattern = new KeyMagicString();
						strPattern->push_back(*itToMatch);
						m_backRef.push_back(*strPattern);
						delete strPattern;
						itToMatch++;
					}
					break;
				case RuleInfo::tNotOfString:
					LOG("tNotOfString\n");
					strPattern = &curRule->stringValue;

					/*for (KeyMagicString::iterator ii = strPattern->begin(); ii != strPattern->end(); ii++) {
						if (*itToMatch == *ii) {
							found = true;
							break;
						}
						found = false;
					}*/
					// if found
					if (strPattern->find(*itToMatch) == KeyMagicString::npos) {
						LOG("--Not matched\n");
						return false;
					} else {
						LOG("--Matched \\%x\n", *itToMatch);
						strPattern = new KeyMagicString();
						strPattern->push_back(*itToMatch);
						m_backRef.push_back(*strPattern);
						delete strPattern;
						itToMatch++;
					}
					break;
				case RuleInfo::tBackRefString: // not yet supported
					//TODO: add backref feature in LHS
					return false;
					break;
				case RuleInfo::tReference: // not yet supported
					//TODO: add reference feature
					return false;
					break;
					/*case ruleInfo::tVKey:
					 if (!(getKeyState(curRule->keyCode) && 1)) {
					 return false;
					 }
					 break;*/
				case RuleInfo::tAny:
					LOG("tAny\n");
					if ( (*itToMatch >= L'\x0021' && *itToMatch <= L'\x007D') || (*itToMatch >= L'\x00FF' && *itToMatch < L'\xFFFF') ) {
						strPattern = new KeyMagicString();
						strPattern->push_back(*itToMatch);
						m_backRef.push_back(*strPattern);
						delete strPattern;
						LOG("--Matched \\%x\n", *itToMatch);
						itToMatch++;
					} else {
						LOG("--Not matched\n");
						return false;
					}
					break;
				case RuleInfo::tSwitch:
					LOG("tSwitch\n");
					if (m_switch.find(curRule->switchId) != m_switch.end()) {
						if (!m_switch[curRule->switchId]) {
							LOG("--switch is off\n");
							return false;
						}
					} else {
						LOG("--switch is off\n");
						return false;
					}

					break;
				default:
					break;
			}
		}
		
		return true;
	}

	bool KeyMagicEngine::processOutput(RuleInfo * rule) {
		LOG_FUNC();
		KeyMagicString outputResult, backupResult;
		
		LOG("process output rule index = %d\n", rule->getRuleIndex());
		
		RuleInfo::ItemList* inRules = rule->getLHS();
//		RuleInfo::ItemList::iterator iInRule = inRules->begin();
		RuleInfo::ItemList* rules = rule->getRHS();
		unsigned int length = rule->getMatchLength();
		
		for (RuleInfo::ItemList::iterator i = rules->begin(); i != rules->end(); i++) {
			RuleInfo::Item * curRule = &*i, * nextRule, *Rule;
			KeyMagicString string;
			long integer;
			
			RuleInfo::ItemList::iterator ii = i+1;
			if (ii == rules->end()) {
				nextRule = NULL;
			} else {
				nextRule = &*ii;
			}
			
			switch (curRule->type) {
				case RuleInfo::tString:
					LOG("tString\n");
					outputResult += curRule->stringValue;
					break;
				case RuleInfo::tReference:
					LOG("tReference\n");
					if (m_backRef.size() <= curRule->refIndex) {
						LOG("--Value is out of index:%d size=%d\n", curRule->refIndex, m_backRef.size());
						return false;
					}
					outputResult += m_backRef.at(curRule->refIndex);
					break;
				case RuleInfo::tBackRefString:
					LOG("tBackRefString\n");
					if (m_backRef.size() <= curRule->refIndex) {
						LOG("--Value is out of index:%d size=%d\n", curRule->refIndex, m_backRef.size());
						return false;
					}
					string = m_backRef.at(curRule->refIndex);
					Rule = &inRules->at(curRule->refIndex);
					integer = Rule->stringValue.find(string);
					if (integer != -1) {
						outputResult += curRule->stringValue.at(integer);
					}
					break;
				case RuleInfo::tVKey:
					LOG("tVKey\n");
					outputResult += curRule->keyCode;
					break;
				case RuleInfo::tSwitch:
					if (m_switch.find(curRule->switchId) != m_switch.end()) {
						m_switch[curRule->switchId] = !m_switch[curRule->switchId];
					} else {
						m_switch[curRule->switchId] = true;
					}
					LOG("tSwitch: %d = %d\n", curRule->switchId, m_switch[curRule->switchId]);
					break;
				default:
					break;
			}
		}
		
		std::string str;
		str = getCharacterReferenceString(m_textContext);
		LOG("m_textContext: %s\n", str.c_str());
		
		str = getCharacterReferenceString(outputResult);
		LOG("outputResult: %s\n", str.c_str());
		
		m_textContext = m_textContext.substr(0, m_textContext.length() - length);
		m_textContext += outputResult;
		
		str = getCharacterReferenceString(m_textContext);
		LOG("m_textContext.length() - length + outputResult: %s\n", str.c_str());
		
		if (outputResult.length() == 0 || (outputResult.length() == 1 && outputResult.at(0) > 0x20 && outputResult.at(0) < 0x7F)) {
			m_shouldMatchAgain = false;
		} else {
			m_shouldMatchAgain = true;
		}
		
		return true;
	}

	void KeyMagicEngine::reset() {
		m_textContext.clear();
		for (unsigned int i = 0; i < KeyStateLen; i++) {
			m_keyStates[i] = 0;
		}
		m_switch.clear();
		m_contextHistory.clear();
	}

	KeyMagicString KeyMagicEngine::getContextText() {
		return m_textContext;
	}

	void KeyMagicEngine::setContextText(const KeyMagicString &textContext) {
		m_textContext.assign(textContext.c_str());
	}

	KeyMagicKeyboard * KeyMagicEngine::getKeyboard() {
		return &m_keyboard;
	}

	std::map<int, bool> KeyMagicEngine::getSwitchMap() {
		return m_switch;
	}

	void KeyMagicEngine::setSwitchMap(const std::map<int, bool> & switchMap) {
		m_switch = switchMap;
	}
	
	TContextHistory KeyMagicEngine::getHistory() {
		return m_contextHistory;
	}
	
	void KeyMagicEngine::setHistory(const TContextHistory &history) {
		m_contextHistory = history;
	}

	unsigned long KeyMagicEngine::getDifference(KeyMagicString const &contextBefore, KeyMagicString *difference) {
		unsigned long deleteCount = 0;

		difference->clear();

		const KeyMagicString& contextAfter = m_textContext;

		if (contextBefore == contextAfter) {
			return deleteCount;
		}

		unsigned long lengthBefore = contextBefore.length();
		unsigned long lengthAfter = contextAfter.length();

		if (contextBefore.length() > contextAfter.length()) {
			deleteCount = contextBefore.length() - contextAfter.length();
			lengthBefore = lengthAfter;
		}

		KeyMagicString::const_iterator bit, ait;
		int match = 0;
		for (
			bit = contextBefore.begin(), ait = contextAfter.begin();
			bit != contextBefore.end() && ait != contextAfter.end();
			bit++, ait++, match++)
		{
			if (*bit != *ait) {
				break;
			}
		}

		if (match < lengthBefore) {
			deleteCount += lengthBefore - match;
		}
		
		if (match < contextAfter.length()) {
			difference->assign(&(contextAfter.c_str()[match]));
		} else {
			difference->assign(L"");
		}


		return deleteCount;
	}
}