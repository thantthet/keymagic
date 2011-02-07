#include <iostream>
#include <vector>
#include <stdio.h>

#include "KeyMagicKeyboard.h"
#include "KeyMagicEngine.h"
#include "KeyMagicLogger.h"

bool KeyMagicEngine::loadKeyboardFile(const char * keyboardFile) {
	bool success;
	
	//load keyboard from file
	success = m_keyboard.loadKeyboardFile(keyboardFile);
	
	//keyboard failed to load ?
	if (!success) {
		return false;
	}
	
	//prepare rules to match
	
	m_strings = m_keyboard.getStrings();
	m_rules = m_keyboard.getRules();
	m_layoutOptions = m_keyboard.getLayoutOptions();
	
	return true;
}

void KeyMagicEngine::updateHistory(KeyMagicString text) {
	if (m_contextHistory.size() == 0) {
		m_contextHistory.push_back(text);
	} else if (m_contextHistory.back() != text) {
		m_contextHistory.push_back(text);
	}
}

bool KeyMagicEngine::processInput(int keyval, int keycode, int modifier) {
	bool success = false;
	
	m_logger->log("processInput;keyval=%c;keycode=%x;modifier=%x\n", keyval, keycode, modifier);

	for (RuleList::iterator i = m_rules->begin(); i != m_rules->end(); i++) {
		RuleInfo * rule = *i;
		success = matchRule(rule, keyval, keycode, modifier);
		if (success) {
			
			if (keycode) {
				m_switch.clear();
			}
			
			// if anything is matched with string pattern, we need to append keyval to the context
			if (!m_matchedVK && keyval != 0) {
				m_textContext += keyval;
			}
			
			processOutput(rule);
			
			//un-press
			m_keyStates[keycode] = 0;
			
			return true;
		}
	}
	return false;
}

bool KeyMagicEngine::processKeyEvent(int keyval, int keycode, int modifier) {
	bool success = false;
	
	m_keyStates[keycode] = 1;
	
	KeyMagicString oldText = m_textContext;
	
	success = processInput(keyval, keycode, modifier);
	
	//un-press
	m_keyStates[keycode] = 0;
	
	if (!success && keycode) {
		m_switch.clear();
	}
	
	if (success) {
		while (success) {
			if (m_shouldMatchAgain) {
				success = processInput(0, 0, 0);
				if (!success) {
					updateHistory(oldText);
				} else if (keycode == 8 && m_contextHistory.size()) {
					if (m_textContext == m_contextHistory.back()) { // if results are the same
						m_contextHistory.pop_back(); // sync with smartbackspace
					}
				}
			} else {
				if (keycode == 8 && m_contextHistory.size()) {
					if (m_textContext == m_contextHistory.back()) { // if results are the same
						m_contextHistory.pop_back(); // sync with smartbackspace
					}
				} else {
					updateHistory(oldText);
				}
				break;
			}
		}
		
		return true;
		
	} else {
		std::string * str = getCharacterReferenceString(&m_textContext); 
		m_logger->log("processInput:FAILED;%s\n", str->c_str());
		delete str;
		
		if ((modifier & CTRL_MASK) || (modifier & ALT_MASK)) {
			return false;
		}
		
		if (keycode == 8) {
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
		//but check if `eat` options is active
		if (keycode == 0x20 || keycode == 0x0D) {
			return false;
		} else if (m_layoutOptions->eat == true) {
			//if yes, eat the key and return as processed
			return true;
		}
	}
	
	return false;
}

int KeyMagicEngine::matchKeyStates(int keycode, int modifier, std::vector<RuleInfo::Item*> * rules) {
	
	int matchedCount = 0;
	int modStates = modifier;
	
	for (std::vector<RuleInfo::Item*>::iterator i = rules->begin(); i != rules->end(); i++) {
		RuleInfo::Item * curRule = *i;
		if (curRule->type == RuleInfo::tVKey) {
			if (curRule->keyCode == 0x10 || curRule->keyCode == 0x11 || curRule->keyCode == 0x12) {
				switch (curRule->keyCode) {
					case 0x10:
						if (modifier & SHIFT_MASK) {
							modStates -= SHIFT_MASK;
							continue;
						} else {
							return -1;
						}
						break;
					case 0x11:
						if (modifier & CTRL_MASK) {
							modStates -= CTRL_MASK;
							continue;
						} else {
							return -1;
						}
						break;
					case 0x12:
						if (modifier & ALT_MASK) {
							modStates -= ALT_MASK;
							continue;
						} else {
							return -1;
						}
						break;
				}
			} else {
				if (keycode != curRule->keyCode) {
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
	KeyMagicString appendedContext = m_textContext;
	
	std::vector<RuleInfo::Item*> * rules = rule->getLHS();
	
	m_matchedVK = false;
	int kcode = matchKeyStates(keycode, modifier, rules);
	if (kcode == -1) {
		return false;
	} else if (kcode == 0) {
		if (keyval != 0) appendedContext += keyval;
	} else {
		m_matchedVK = true;
	}
	
	unsigned int lenToMatch = rule->getMatchLength();
	unsigned int lenAppended = appendedContext.length();
	
	if (lenToMatch > lenAppended) {
		return false;
	}
	
	//get from the end
	KeyMagicString stringToMatch = appendedContext.substr(lenAppended - lenToMatch);
	KeyMagicString::iterator itToMatch = stringToMatch.begin();
	
	m_backRef.clear();
	
	for (std::vector<RuleInfo::Item*>::iterator i = rules->begin(); i != rules->end(); i++) {
		RuleInfo::Item * curRule = *i, * nextRule;
		KeyMagicString * strPattern;
		bool found;
		
		std::vector<RuleInfo::Item*>::iterator ii = i+1;
		if (ii == rules->end()) {
			nextRule = NULL;
		} else {
			nextRule = *ii;
		}
		
		switch (curRule->type) {
			case RuleInfo::tString:
				strPattern = curRule->stringValue;
				for (KeyMagicString::iterator ii = strPattern->begin(); ii != strPattern->end(); ii++) {
					// if itMatch is at the end
					if (itToMatch == stringToMatch.end()) {
						return false;
						// if not match
					} else if (*itToMatch++ != *ii) {
						return false;
					}
				}
				m_backRef.push_back(*strPattern);
				break;
			case RuleInfo::tAnyOfString:
				strPattern = curRule->stringValue;
				for (KeyMagicString::iterator ii = strPattern->begin(); ii != strPattern->end(); ii++) {
					if (*itToMatch == *ii) {
						found = true;
						strPattern = new KeyMagicString();
						strPattern->push_back(*ii);
						m_backRef.push_back(*strPattern);
						break;
					}
					found = false;
				}
				// if not matched
				if (!found) {
					return false;
				} else {
					itToMatch++;
				}
				break;
			case RuleInfo::tNotOfString:
				strPattern = curRule->stringValue;
				for (KeyMagicString::iterator ii = strPattern->begin(); ii != strPattern->end(); ii++) {
					if (*itToMatch == *ii) {
						found = true;
						strPattern = new KeyMagicString();
						strPattern->push_back(*ii);
						m_backRef.push_back(*strPattern);
						break;
					}
					found = false;
				}
				// if matched
				if (found) {
					return false;
				} else {
					itToMatch++;
				}
				break;
			case RuleInfo::tBackRefString: // not yet supported
				//TODO add backref feature in LHS
				return false;
				break;
			case RuleInfo::tReference: // not yet supported
				//TODO
				return false;
				break;
				/*case ruleInfo::tVKey:
				 if (!(getKeyState(curRule->keyCode) && 1)) {
				 return false;
				 }
				 break;*/
			case RuleInfo::tAny:
				itToMatch++;
				break;
			case RuleInfo::tSwitch:
				if (m_switch.find(curRule->switchId) != m_switch.end()) {
					if (!m_switch[curRule->switchId]) {
						return false;
					}
				} else {
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
	KeyMagicString outputResult, backupResult;
	
	m_logger->log("processOutput;%d\n", rule->getRuleIndex());
	
	std::vector<RuleInfo::Item*> * inRules = rule->getLHS();
	std::vector<RuleInfo::Item*>::iterator iInRule = inRules->begin();
	std::vector<RuleInfo::Item*> * rules = rule->getRHS();
	unsigned int length = rule->getMatchLength();
	
	for (std::vector<RuleInfo::Item*>::iterator i = rules->begin(); i != rules->end(); i++) {
		RuleInfo::Item * curRule = *i, * nextRule, *Rule;
		KeyMagicString string;
		int integer;
		
		std::vector<RuleInfo::Item*>::iterator ii = i+1;
		if (ii == rules->end()) {
			nextRule = NULL;
		} else {
			nextRule = *ii;
		}
		
		switch (curRule->type) {
			case RuleInfo::tString:
				outputResult += *curRule->stringValue;
				break;
			case RuleInfo::tReference:
				if (m_backRef.size() <= curRule->refIndex) {
					return false;
				}
				outputResult += m_backRef.at(curRule->refIndex);
				//TODO
				break;
			case RuleInfo::tBackRefString:
				if (m_backRef.size() <= curRule->refIndex) {
					return false;
				}
				string = m_backRef.at(curRule->refIndex);
				Rule = inRules->at(curRule->refIndex);
				integer = Rule->stringValue->find(string);
				if (integer != -1) {
					outputResult += curRule->stringValue->at(integer);
				}
				break;
			case RuleInfo::tVKey:
				outputResult += curRule->keyCode;
				break;
			case RuleInfo::tSwitch:
				m_switch[curRule->switchId] = !m_switch[curRule->switchId];
				break;
			default:
				break;
		}
	}
	
	m_textContext = m_textContext.substr(0, m_textContext.length() - length);
	m_textContext += outputResult;
	
	if (outputResult.length() == 0 || (outputResult.length() == 1 && outputResult.at(0) > 0x20 && outputResult.at(0) < 0x7F)) {
		m_shouldMatchAgain = false;
	} else {
		m_shouldMatchAgain = true;
	}
	
	return true;
}
