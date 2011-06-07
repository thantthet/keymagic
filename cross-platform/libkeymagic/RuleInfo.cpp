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

#include "RuleInfo.h"
#include "KeyCodes.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace libkm {

RuleInfo::Item::Item(types t, const KeyMagicString * string) {
	stringValue = new KeyMagicString();
	stringValue->assign(string->c_str());
	type = t;
}

RuleInfo::Item::Item(types t, int i) {
	stringValue = NULL;
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

RuleInfo::Item::Item (types t) {
	stringValue = NULL;
	type = t;
}

RuleInfo::Item::~Item() {
	if (stringValue != NULL) {
		delete stringValue;
	}
}

RuleInfo::RuleInfo(short * in, short * out, StringList * variable)
: m_index(0), m_countSwitch(0), m_countVkey(0), m_matchLength(0), m_logger(KeyMagicLogger::getInstance()) {
	
	int length;
	length = toRuleInfo(in, &m_lhsRule, variable);
	if (length == -1) {
		// ERROR
		m_lhsRule.clear();
		m_logger->log("Invalid lhs rule");
	}
	m_matchLength = length;
	m_countSwitch = getSwitchCount(&m_lhsRule);
	m_countVkey = getVKCount(&m_lhsRule);

	length = toRuleInfo(out, &m_rhsRule, variable);
	if (length == -1) {
		// ERROR
		m_rhsRule.clear();
		m_logger->log("Invalid rhs rule");
	}
}

RuleInfo::~RuleInfo() {
	for (std::vector<Item*>::iterator i = m_rhsRule.begin(); i != m_rhsRule.end(); i++) {
		delete *i;
	}
	m_rhsRule.clear();
	for (std::vector<Item*>::iterator i = m_lhsRule.begin(); i != m_lhsRule.end(); i++) {
		delete *i;
	}
	m_lhsRule.clear();
}

int RuleInfo::getVKCount(std::vector<Item*> * rule) {
	int countVK = 0;
	
	for (std::vector<Item*>::iterator i = rule->begin(); i != rule->end(); i++) {
		Item * item = *i;
		switch (item->type) {
			case tVKey:
				countVK++;
				break;
			default:
				break;
		}
	}
	
	return countVK;
}

int RuleInfo::getSwitchCount(std::vector<Item*> * rule) {
	int countSwitch = 0;
	
	for (std::vector<Item*>::iterator i = rule->begin(); i != rule->end(); i++) {
		Item * item = *i;
		switch (item->type) {
			case tSwitch:
				countSwitch++;
				break;
			default:
				break;
		}
	}
	
	return countSwitch;
}

std::string * RuleInfo::description() {
	std::string *s, *str;
	
	std::stringstream ss;
	
	ss << "length: " << m_matchLength << ",\n";
	ss << "lhs:[\n";
	s = JSON(&m_lhsRule);
	ss << s->c_str(); delete s;
	ss << "\n],\n";
	
	ss << "rhs:[\n";
	s = JSON(&m_rhsRule);
	ss << (s->c_str()); delete s;
	ss << "\n]\n";
	str = new std::string(ss.str());
	return str;
}

std::string * RuleInfo::JSON(std::vector<Item*> * rule) {
	std::string * str;
	std::stringstream ss;
	
	for (std::vector<Item*>::iterator i = rule->begin(); i != rule->end(); i++) {
		Item * item = *i;
		std::string * s;
		
		if (i != rule->begin()) {
			ss << ",\n";
		}
		
		switch (item->type) {
			case tString:
				s = getCharacterReferenceString(item->stringValue);
				ss << "{ type:'tString', 'value':'" << s->c_str() << "' }";
				delete s;
				break;
			case tAnyOfString:
				s = getCharacterReferenceString(item->stringValue);
				ss << "{ type:'tAnyOfString', 'value':'" << s->c_str() << "' }";
				delete s;
				break;
			case tNotOfString:
				s = getCharacterReferenceString(item->stringValue);
				ss << "{ type:'tNotOfString', 'value':'" << s->c_str() << "' }";
				delete s;
				break;
			case tBackRefString:
				s = getCharacterReferenceString(item->stringValue);
				ss << "{ type:'tBackRefString', 'index':" << std::dec << item->refIndex << ", 'value':'" << s->c_str() << "' }";
				delete s;
				break;
			case tReference:
				ss << "{ type:'tReference', 'index':" << std::dec << item->refIndex << " }";
				break;
			case tVKey:
				ss << "{ type:'tVKey', 'keyCode':0x" << std::hex << item->keyCode << " }";
				break;
			case tAny:
				ss << "{ type:'tAny' }";
				break;
			case tSwitch:
				ss << "{ type:'tSwitch', 'switchId':" << std::dec << item->switchId << " }";
				break;
			default:
				ss << "{ type=tUnknown }";
				break;
		}
	}
	str = new std::string(ss.str());
	return str;
}

int RuleInfo::toRuleInfo(short * binRule, std::vector<Item*> * outRule, StringList * variable) {
	int size = 0, index = 0, mode = 0, patLength = 0;
	KeyCodes keyCodes;

	while (*binRule) {
		Item * item;
		KeyMagicString string;

		switch (*binRule++) {
		case opSTRING:
			size = *binRule++;
			for (int i = 0; i < size; i++) {
				string += *binRule++;
			}
			outRule->push_back(new Item(tString, &string));
			patLength += string.length();
			break;
		case opVARIABLE:
			index = *binRule++;
			index--;

			if (index >= variable->size()) {
				return -1;
			}
			string.assign(variable->at(index));
			outRule->push_back(new Item(tString, &string));
			patLength += string.length();
			break;
		case opREFERENCE: // not yet support on the lhs
			index = *binRule++;
			outRule->push_back(new Item(tReference, index - 1));
			break;
		case opPREDEFINED:
			outRule->push_back(new Item(tString, &keyCodes.getKeyValue(*binRule++)));
			patLength++;
			break;
		case opMODIFIER:
			if (outRule->size() == 0) {
				m_logger->log("ERROR: Illegal usage of modifier.");
				return -1;
			}
			patLength -= outRule->back()->stringValue->length();
			patLength++;

			mode = *binRule++;
			//mode may be 'any of' or 'not any of' or 'reference index'
			if (mode == opANYOF) {
				item = outRule->back();
				item->type = tAnyOfString;
			} else if (mode == opNANYOF) {
				item = outRule->back();
				item->type = tNotOfString;
			} else {
				index = mode;
				item = outRule->back();
				item->type = tBackRefString;
				item->refIndex = index - 1;
			}
			break;
		case opAND:
			if (*binRule++ == opPREDEFINED) {
				m_countVkey++;
				outRule->push_back(new Item(tVKey, keyCodes.getKeyValue(*binRule++).at(0)));
				while(*binRule++ == opPREDEFINED) {
					outRule->push_back(new Item(tVKey, keyCodes.getKeyValue(*binRule++).at(0)));
				}
				return patLength;
			} else {
				m_logger->log("ERROR: OP is not predefined.");
				return -1;
			}
			break;
		case opANY:
			patLength++;
			outRule->push_back(new Item(tAny));
			break;
		case opSWITCH:
			outRule->push_back(new Item(tSwitch, *binRule++));
			break;
		default:
			m_logger->log("ERROR: Unknown OP is found.\n");
			break;
		}
	}

	return patLength;
}
}