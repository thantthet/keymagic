/*
 * KeyMagicLogger.cpp
 *
 *  Created on: Jan 28, 2011
 *      Author: thantthetkz
 */

#include "KeyMagicLogger.h"

KeyMagicLogger* KeyMagicLogger::m_instance = NULL;

KeyMagicLogger::KeyMagicLogger() {
	m_logFile = stderr;
}

KeyMagicLogger * KeyMagicLogger::getInstance() {
	if (m_instance == NULL) {
		m_instance = new KeyMagicLogger();
	}
	return m_instance;
}

void KeyMagicLogger::log(const char * fmt, ...) {
	va_list vl;
	
	va_start (vl, fmt);
	vfprintf(m_logFile, fmt, vl);
	va_end(vl);
}

FILE * KeyMagicLogger::getFile() {
	return m_logFile;
}

void KeyMagicLogger::setFile(FILE * file) {
	m_logFile = file;
}