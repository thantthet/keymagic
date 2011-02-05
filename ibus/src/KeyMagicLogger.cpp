/*
 * KeyMagicLogger.cpp
 *
 *  Created on: Jan 28, 2011
 *      Author: thantthetkz
 */

#include "KeyMagicLogger.h"

KeyMagicLogger::KeyMagicLogger() {

}

void KeyMagicLogger::log(const wchar_t * msg) {
	print(msg);
}

void KeyMagicLogger::log(const KeyMagicString * msg) {
	print(msg->c_str());
}

void KeyMagicLogger::print(const wchar_t * msg) {
}