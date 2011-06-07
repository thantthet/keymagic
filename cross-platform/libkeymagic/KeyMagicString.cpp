/*
 * KeyMagicString.cpp
 *
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

#include <sstream>
#include "KeyMagicString.h"

namespace libkm {

std::string * getCharacterReferenceString(const KeyMagicString * text) {
	std::ostringstream oss;
	std::string * str;
	for (KeyMagicString::const_iterator i = text->begin(); i != text->end(); i++) {
		oss << "\\u" << std::hex << (*i <= 0xfff ? "0" : "") << (*i <= 0xff ? "0" : "") << *i;
	}
	str = new std::string(oss.str());
	return str;
}
}