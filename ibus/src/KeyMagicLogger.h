/*
 * KeyMagicLogger.h
 *
 *  Created on: Jan 28, 2011
 *      Author: thantthetkz
 */

#ifndef KEYMAGICLOGGER_H_
#define KEYMAGICLOGGER_H_

#include <iostream>
#include "KeyMagicTypes.h"

#pragma GCC visibility push(default)

class KeyMagicLogger {
public:
	static KeyMagicLogger* getInstance();
	void log(const char * fmt, ...);
	FILE * getFile();
	void setFile(FILE * file);
	~KeyMagicLogger() {
		m_instance = NULL;
	}
private:
	KeyMagicLogger();
	
	static KeyMagicLogger * m_instance;
	FILE * m_logFile;
};

#pragma GCC visibility pop

#endif /* KEYMAGICLOGGER_H_ */
