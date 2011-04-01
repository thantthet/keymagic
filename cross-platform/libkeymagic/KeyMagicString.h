#ifndef KEYMAGICSTRING_H_
#define KEYMAGICSTRING_H_

#include <string>

typedef std::basic_string<wchar_t> KeyMagicString;

std::string * getCharacterReferenceString(const KeyMagicString * text);

#endif
