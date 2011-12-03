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
		stringValue = string->c_str();
		type = t;
	}

	RuleInfo::Item::Item(types t, unsigned int i) {
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
		type = t;
	}

	RuleInfo::RuleInfo(const short * in, const short * out, StringList * variable)
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
		/*for (ItemList::iterator i = m_rhsRule.begin(); i != m_rhsRule.end(); i++) {
			delete *i;
		}*/
		m_rhsRule.clear();
		/*for (ItemList::iterator i = m_lhsRule.begin(); i != m_lhsRule.end(); i++) {
			delete *i;
		}*/
		m_lhsRule.clear();
	}
	
	bool RuleInfo::operator < (RuleInfo& r2) {
		return this->operator <((const RuleInfo&) r2);
	}

	bool RuleInfo::operator < (const RuleInfo& r2) const {

		int s1 = this->m_countSwitch;
		int s2 = r2.m_countSwitch;
		if (s1 != s2) {
			return s1 < s2;
		}

		int v1 = this->m_countVkey;
		int v2 = r2.m_countVkey;
		if (v1 != v2) {
			return v1 < v2;
		}

		int m1 = this->m_matchLength;
		int m2 = r2.m_matchLength;
		if (m1 != m2) {
			return m1 < m2;
		}

		return false;
	}

	int RuleInfo::getVKCount(ItemList * rule) {
		int countVK = 0;
		
		for (ItemList::iterator i = rule->begin(); i != rule->end(); i++) {
			Item item = *i;
			switch (item.type) {
				case tVKey:
					countVK++;
					break;
				default:
					break;
			}
		}
		
		return countVK;
	}

	int RuleInfo::getSwitchCount(ItemList * rule) {
		int countSwitch = 0;
		
		for (ItemList::iterator i = rule->begin(); i != rule->end(); i++) {
			Item item = *i;
			switch (item.type) {
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

	std::string * RuleInfo::JSON(ItemList * rule) {
		std::string * str;
		std::stringstream ss;
		
		for (ItemList::iterator i = rule->begin(); i != rule->end(); i++) {
			Item item = *i;
			std::string s;
			
			if (i != rule->begin()) {
				ss << ",\n";
			}
			
			switch (item.type) {
				case tString:
					s = getCharacterReferenceString(item.stringValue);
					ss << "{ type:'tString', 'value':'" << s.c_str() << "' }";
					break;
				case tAnyOfString:
					s = getCharacterReferenceString(item.stringValue);
					ss << "{ type:'tAnyOfString', 'value':'" << s.c_str() << "' }";
					break;
				case tNotOfString:
					s = getCharacterReferenceString(item.stringValue);
					ss << "{ type:'tNotOfString', 'value':'" << s.c_str() << "' }";
					break;
				case tBackRefString:
					s = getCharacterReferenceString(item.stringValue);
					ss << "{ type:'tBackRefString', 'index':" << std::dec << item.refIndex << ", 'value':'" << s.c_str() << "' }";
					break;
				case tReference:
					ss << "{ type:'tReference', 'index':" << std::dec << item.refIndex << " }";
					break;
				case tVKey:
					ss << "{ type:'tVKey', 'keyCode':0x" << std::hex << item.keyCode << " }";
					break;
				case tAny:
					ss << "{ type:'tAny' }";
					break;
				case tSwitch:
					ss << "{ type:'tSwitch', 'switchId':" << std::dec << item.switchId << " }";
					break;
				default:
					ss << "{ type=tUnknown }";
					break;
			}
		}
		str = new std::string(ss.str());
		return str;
	}

	int RuleInfo::toRuleInfo(const short * binRule, ItemList * outRule, StringList * variable) {
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
				item = new Item(tString, &string);
				outRule->push_back(*item);
				delete item;
				patLength += string.length();
				break;
			case opVARIABLE:
				index = *binRule++;
				index--;

				if (index >= variable->size()) {
					return -1;
				}
				string.assign(variable->at(index));
				item = new Item(tString, &string);
				outRule->push_back(*item);
				delete item;
				patLength += string.length();
				break;
			case opREFERENCE: // not yet support on the lhs
				index = *binRule++;
				item = new Item(tReference, index - 1);
				outRule->push_back(*item);
				delete item;
				break;
			case opPREDEFINED:
				item = new Item(tString, &keyCodes.getKeyValue(*binRule++));
				outRule->push_back(*item);
				delete item;
				patLength++;
				break;
			case opMODIFIER:
				if (outRule->size() == 0) {
					m_logger->log("ERROR: Illegal usage of modifier.");
					return -1;
				}
				patLength -= outRule->back().stringValue.length();
				patLength++;

				mode = *binRule++;
				//mode may be 'any of' or 'not any of' or 'reference index'
				if (mode == opANYOF) {
					outRule->back().type = tAnyOfString;
				} else if (mode == opNANYOF) {
					outRule->back().type = tNotOfString;
				} else {
					index = mode;
					outRule->back().type = tBackRefString;
					outRule->back().refIndex = index - 1;
				}
				break;
			case opAND:
				if (*binRule++ == opPREDEFINED) {
					m_countVkey++;
					item = new Item(tVKey, keyCodes.getKeyValue(*binRule++).at(0));
					outRule->push_back(*item);
					delete item;
					while(*binRule++ == opPREDEFINED) {
						item = new Item(tVKey, keyCodes.getKeyValue(*binRule++).at(0));
						outRule->push_back(*item);
						delete item;
					}
					return patLength;
				} else {
					m_logger->log("ERROR: OP is not predefined.");
					return -1;
				}
				break;
			case opANY:
				patLength++;
				item = new Item(tAny);
				outRule->push_back(*item);
				delete item;
				break;
			case opSWITCH:
				item = new Item(tSwitch, *binRule++);
				outRule->push_back(*item);
				delete item;
				break;
			default:
				m_logger->log("ERROR: Unknown OP is found.\n");
				break;
			}
		}

		return patLength;
	}
}