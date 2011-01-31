#include "RuleInfo.h"
#include "KeyCodes.h"
#include <iostream>
#include <fstream>

RuleInfo::RuleInfo(short * in, short * out, StringList * variable)
: m_index(0), m_countSwitch(0), m_countVkey(0), m_matchLength(0) {
	int length;
	length = toRuleInfo(in, &m_lhsRule, variable);
	if (length == -1) {
		// ERROR
	}
	m_matchLength = length;

	length = toRuleInfo(out, &m_rhsRule, variable);
	if (length == -1) {
		// ERROR
	}
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
				m_countSwitch++;
				outRule->push_back(new Item(tVKey, keyCodes.getKeyValue(*binRule++)->at(0)));
				while(*binRule++ == opPREDEFINED) {
					m_countSwitch++;
					outRule->push_back(new Item(tVKey, keyCodes.getKeyValue(*binRule++)->at(0)));
				}
			} else {
				return false;
			}
			break;
		case opANY:
			outRule->push_back(new Item(tAny));
			break;
		case opSWITCH:
			outRule->push_back(new Item(tSwitch, *binRule++));
			m_countVkey++;
			break;
		}
	}

	return patLength;
}
