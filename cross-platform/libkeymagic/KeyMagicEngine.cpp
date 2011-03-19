#include <iostream>
#include <vector>
#include <stdio.h>

#include "KeyMagicKeyboard.h"
#include "KeyMagicEngine.h"
#include "KeyMagicLogger.h"

KeyMagicEngine::KeyMagicEngine() :
	m_logger(KeyMagicLogger::getInstance()),
	m_verbose(0),
	m_haveKeyboard(0)
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
	
	//prepare rules to match
	
	m_strings = m_keyboard.getStrings();
	m_rules = m_keyboard.getRules();
	m_layoutOptions = m_keyboard.getLayoutOptions();
	
	//now we have vaild keyboard in memory
	m_haveKeyboard = true;
	
	return true;
}

void KeyMagicEngine::updateHistory(KeyMagicString text) {
	std::string * str;
	
	str = getCharacterReferenceString(&text);
	LOG("updateHistory=>text:%s ", str->c_str());
	delete str;
	
	LOG("m_contextHistory.size=%d ", m_contextHistory.size());
	if (m_contextHistory.size() == 0) {
		m_contextHistory.push_back(text);
		LOG("\n");
	} else if (text.compare(m_contextHistory.back()) != 0) {
		str = getCharacterReferenceString(&m_contextHistory.back()); 
		LOG("m_contextHistory.back():%s\n", str->c_str());
		delete str;
		m_contextHistory.push_back(text);
	}
}

bool KeyMagicEngine::processInput(int keyval, int keycode, int modifier) {
	bool success = false;
	
	LOG("processInput: keycode=%x; modifier=%x; keyval=%c\n", keycode, modifier, keyval);

	for (RuleList::iterator i = m_rules->begin(); i != m_rules->end(); i++) {
		RuleInfo * rule = *i;
		success = matchRule(rule, keyval, keycode, modifier);
		if (success) {
			
			LOG("rule matched: %d\n", rule->getRuleIndex());
			
			if (keycode) {
				m_switch.clear();
			}
			
			// if anything is matched with string pattern, we need to append keyval to the context
			if (!m_matchedVK && keyval != 0) {
				LOG("m_matchedVK: %d\n", m_matchedVK);
				m_textContext += keyval;
			}
			
			success = processOutput(rule);
			
			//un-press
			m_keyStates[keycode] = 0;
			
			return success;
		}
	}
	return false;
}

bool KeyMagicEngine::processKeyEvent(int keyval, int keycode, int modifier) {
	bool success = false;
	
	//if there is no keyboard loaded
	if (m_haveKeyboard == false) {
		return false;
	}
	
	m_keyStates[keycode] = 1;
	
	KeyMagicString oldText = m_textContext;
	
	if (m_keyboard.getLayoutOptions()->trackCaps == true) {
		//TODO: handle trackCaps option
	}
	
	success = processInput(keyval, keycode, modifier);
	
	//un-press
	m_keyStates[keycode] = 0;
	
	if (!success && keycode) {
		m_switch.clear();
	}
	int looped = 0;
	if (success) {
		while (success) {
			if (m_shouldMatchAgain) {
				success = processInput(0, 0, 0);
				LOG("processInput(0, 0, 0)=>%d\n", success);
				
				looped++;
				if (looped == 20) {
					LOG("Maximum matching loop exceeded.\n");
					return false;
				}
				
				if (!success) {
					updateHistory(oldText);
					return true;
				} else if (keycode == 8 && m_contextHistory.size()) {
					if (m_textContext == m_contextHistory.back()) { // if results are the same
						m_contextHistory.pop_back(); // sync with smartbackspace
					} else {
						m_contextHistory.clear();
					}

				}
			} else if (keycode == 8 && m_contextHistory.size()) {
				if (m_textContext == m_contextHistory.back()) { // if results are the same
					m_contextHistory.pop_back(); // sync with smartbackspace
				} else {
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
		std::string * str = getCharacterReferenceString(&m_textContext); 
		LOG("processInput:FAILED;%s\n", str->c_str());
		delete str;
		
		if ((modifier & CTRL_MASK) || (modifier & ALT_MASK)) {
			return false;
		}
		
		if (keycode == 8) {
			LOG("autoBksp=%d;m_contextHistory.size=%d\n", m_layoutOptions->autoBksp, m_contextHistory.size());
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
		if (keycode <= 0x20 || keyval == 0) {
			return false;
		} else if (m_layoutOptions->eat == true) {
			//if yes, eat the key and return as processed
			return true;
		} else {
			m_textContext += keyval;
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
	LOG("==Matching rule=%d==\n", rule->getRuleIndex());
	KeyMagicString appendedContext = m_textContext;
	
	std::vector<RuleInfo::Item*> * rules = rule->getLHS();
	
	m_matchedVK = false;
	int kcode = matchKeyStates(keycode, modifier, rules);
	if (kcode == -1) {
		LOG("Key-states do not matched\n");
		return false;
	} else if (kcode == 0) {
		if (keyval != 0) appendedContext += keyval;
	} else {
		m_matchedVK = true;
	}
	
	unsigned int lenToMatch = rule->getMatchLength();
	unsigned int lenAppended = appendedContext.length();
	
	if (lenToMatch > lenAppended) {
		LOG("Not enough length to match\n");
		return false;
	}
	
	//get from the end
	KeyMagicString stringToMatch = appendedContext.substr(lenAppended - lenToMatch);
	KeyMagicString::iterator itToMatch = stringToMatch.begin();
	
	m_backRef.clear();
	
	for (std::vector<RuleInfo::Item*>::iterator i = rules->begin(); i != rules->end(); i++) {
		RuleInfo::Item * curRule = *i, * nextRule;
		KeyMagicString * strPattern;
		//bool found;
		
		std::vector<RuleInfo::Item*>::iterator ii = i+1;
		if (ii == rules->end()) {
			nextRule = NULL;
		} else {
			nextRule = *ii;
		}
		
		switch (curRule->type) {
			case RuleInfo::tString:
				LOG("tString\n");
				strPattern = curRule->stringValue;
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
				strPattern = curRule->stringValue;

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
				strPattern = curRule->stringValue;

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
	KeyMagicString outputResult, backupResult;
	
	LOG("processOutput: %d\n", rule->getRuleIndex());
	
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
				LOG("tString\n");
				outputResult += *curRule->stringValue;
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
				Rule = inRules->at(curRule->refIndex);
				integer = Rule->stringValue->find(string);
				if (integer != -1) {
					outputResult += curRule->stringValue->at(integer);
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
				LOG("tSwitch:%d=%d\n", curRule->switchId, m_switch[curRule->switchId]);
				break;
			default:
				break;
		}
	}
	
	std::string * str;
	str = getCharacterReferenceString(&m_textContext);
	LOG("processOutput: m_textContext: %s\n", str->c_str());
	delete str;
	
	str = getCharacterReferenceString(&outputResult);
	LOG("processOutput: outputResult: %s\n", str->c_str());
	delete str;
	
	m_textContext = m_textContext.substr(0, m_textContext.length() - length);
	m_textContext += outputResult;
	
	str = getCharacterReferenceString(&m_textContext);
	LOG("processOutput: m_textContext.length() - length + outputResult: %s\n", str->c_str());
	delete str;
	
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

KeyMagicString * KeyMagicEngine::getContextText() {
	return &m_textContext;
}

void KeyMagicEngine::setContextText(KeyMagicString * textContext) {
	m_textContext.assign(textContext->c_str());
}

KeyMagicKeyboard * KeyMagicEngine::getKeyboard() {
	return &m_keyboard;
}