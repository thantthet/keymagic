#ifndef KEYMAGICSTRING_H_
#define KEYMAGICSTRING_H_

#include <string>

#pragma GCC visibility push(default)

typedef std::basic_string<wchar_t> KeyMagicString;

std::string * getCharacterReferenceString(KeyMagicString * text);

#pragma GCC visibility pop

#endif
