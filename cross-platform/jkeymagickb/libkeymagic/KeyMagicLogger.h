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

/**
 * Logger
 */
class KeyMagicLogger {
public:
	/**
	 * Get instance of class
	 */
	static KeyMagicLogger* getInstance();
	/**
	 * Log to file
	 * @param fmt Format
	 */
	void log(const char * fmt, ...);
	/**
	 * Get current destination file
	 */
	FILE * getFile();
	/**
	 * Set file to output
	 */
	void setFile(FILE * file);
	/**
	 * Flush
	 */
	void flush();
	/**
	 * Destructor
	 */
	~KeyMagicLogger();
private:
	KeyMagicLogger();
	
	static KeyMagicLogger * m_instance;
	FILE * m_logFile;
};

#pragma GCC visibility pop

#define LOG if (m_verbose) m_logger->log

#endif /* KEYMAGICLOGGER_H_ */
