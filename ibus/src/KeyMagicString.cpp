#include <sstream>
#include "KeyMagicString.h"

std::string * getCharacterReferenceString(KeyMagicString * text) {
	std::ostringstream oss;
	std::string * str;
	for (KeyMagicString::iterator i = text->begin(); i != text->end(); i++) {
		oss << "\\u" << std::hex << (*i <= 0xfff ? "0" : "") << (*i <= 0xff ? "0" : "") << *i;
	}
	str = new std::string(oss.str());
	return str;
}