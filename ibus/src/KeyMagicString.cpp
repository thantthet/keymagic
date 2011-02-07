#include <sstream>
#include "KeyMagicString.h"

std::string * getCharacterReferenceString(KeyMagicString * text) {
	std::ostringstream oss;
	std::string * str;
	for (KeyMagicString::iterator i = text->begin(); i != text->end(); i++) {
		oss << "\\x" << std::hex << *i;
	}
	str = new std::string(oss.str());
	return str;
}