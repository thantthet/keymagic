/*
 * KeyMagicLogger.h
 *
 *  Created on: Jan 28, 2011
 *      Author: thantthetkz
 */

#ifndef KEYMAGICLOGGER_H_
#define KEYMAGICLOGGER_H_

#include <iostream>
#include <KeyMagicTypes.h>

class KeyMagicLogger : public std::wostream {
public:
	KeyMagicLogger();
	void log(const wchar_t * msg);
	void log(const KeyMagicString * msg);
private:
	void print(const wchar_t * msg);
};

#endif /* KEYMAGICLOGGER_H_ */
