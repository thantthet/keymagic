#include "RuleInfo.h"
#include "KeyCodes.h"
#include <iostream>
#include <fstream>
#include <sstream>

RuleInfo::RuleInfo(short * in, short * out, StringList * variable)
: m_index(0), m_countSwitch(0), m_countVkey(0), m_matchLength(0), m_logger(KeyMagicLogger::getInstance()) {
	
	int length;
	length = toRuleInfo(in, &m_lhsRule, variable);
	if (length == -1) {
		// ERROR
		m_lhsRule.clear();
		m_logger->log("invalid lhs rule");
	}
	m_matchLength = length;

	length = toRuleInfo(out, &m_rhsRule, variable);
	if (length == -1) {
		// ERROR
		m_rhsRule.clear();
		m_logger->log("invalid rhs rule");
	}
}

std::string * RuleInfo::description() {
	std::string *s, *str = new std::string();
	str->append("LHS:\n");
	s = readable(&m_lhsRule);
	str->append(s->c_str()); delete s;
	
	str->append("RHS:\n");
	s = readable(&m_rhsRule);
	str->append(s->c_str()); delete s;
	return str;
}

std::string * RuleInfo::readable(std::vector<Item*> * rule) {
	std::string * str;
	std::stringstream ss;
	
	for (std::vector<Item*>::iterator i = rule->begin(); i != rule->end(); i++) {
		Item * item = *i;
		std::string * s;
		switch (item->type) {
			case tString:
				s = getCharacterReferenceString(item->stringValue);
				ss << "{ type=tString; value=" << s->c_str() << " }\n";
				delete s;
				break;
			case tAnyOfString:
				s = getCharacterReferenceString(item->stringValue);
				ss << "{ type=tAnyOfString; value=" << s->c_str() << " }\n";
				delete s;
				break;
			case tNotOfString:
				s = getCharacterReferenceString(item->stringValue);
				ss << "{ type=tNotOfString; value=" << s->c_str() << " }\n";
				delete s;
				break;
			case tBackRefString:
				ss << "{ type=tBackRefString; value=" << item->refIndex << " }\n";
				break;
			case tReference:
				ss << "{ type=tReference; value=" << item->refIndex << " }\n";
				break;
			case tVKey:
				ss << "{ type=tVKey; value=" << item->keyCode << " }\n";
				break;
			case tAny:
				ss << "{ type=tAny }\n";
				break;
			case tSwitch:
				ss << "{ type=tSwitch; value=" << item->switchId << " }\n";
				break;
			default:
				ss << "{ type=unknown }\n";
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
			outRule->push_back(new Item(tString, keyCodes.getKeyValue(*binRule++)));
			patLength++;
			break;
		case opMODIFIER:
			if (outRule->size() == 0) {
				return false;
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
				outRule->push_back(new Item(tVKey, keyCodes.getKeyValue(*binRule++)->at(0)));
				while(*binRule++ == opPREDEFINED) {
					m_countVkey++;
					outRule->push_back(new Item(tVKey, keyCodes.getKeyValue(*binRule++)->at(0)));
				}
				return patLength;
			} else {
				return false;
			}
			break;
		case opANY:
			outRule->push_back(new Item(tAny));
			break;
		case opSWITCH:
			outRule->push_back(new Item(tSwitch, *binRule++));
			m_countSwitch++;
			break;
		default:
			m_logger->log("Invalid OP\n");
			break;
		}
	}

	return patLength;
}
