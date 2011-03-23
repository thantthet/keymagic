/*
 * KeyMagicErrorLogger.h
 *
 *  Created on: March 20, 2011
 *      Author: thantthetkz
 */

#ifndef KEYMAGICERRORLOGGER_H_
#define KEYMAGICERRORLOGGER_H_

#include "LogFileWriter.h"

/**
 * KeyMagicErrorLogger Class
 */
class KeyMagicErrorLogger : public LogFileWriter {
public:
	/**
	 * Get instance of class
	 */
	static KeyMagicErrorLogger* getInstance();
	/**
	 * Destructor
	 */
	~KeyMagicErrorLogger();
private:
	KeyMagicErrorLogger();
	static KeyMagicErrorLogger * m_instance;
};

#define PERROR KeyMagicErrorLogger::getInstance()->log

#endif