/*
 * KeyMagicLogger.h
 *
 *  Created on: Jan 28, 2011
 *      Author: thantthetkz
 */

#ifndef KEYMAGICLOGGER_H_
#define KEYMAGICLOGGER_H_

#include "LogFileWriter.h"

/**
 * Logger
 */
class KeyMagicLogger : public LogFileWriter {
public:
	/**
	 * Get instance of class
	 */
	static KeyMagicLogger* getInstance();
	/**
	 * Destructor
	 */
	~KeyMagicLogger();
private:
	KeyMagicLogger();
	
	static KeyMagicLogger * m_instance;
};

#define LOG if (m_verbose) m_logger->log

#endif /* KEYMAGICLOGGER_H_ */
