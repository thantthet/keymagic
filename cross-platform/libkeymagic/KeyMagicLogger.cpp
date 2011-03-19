/*
 * KeyMagicLogger.cpp
 *
 *  Created on: Jan 28, 2011
 *      Author: thantthetkz
 */

#include <stdio.h>
#include <stdarg.h>
#include "KeyMagicLogger.h"

KeyMagicLogger* KeyMagicLogger::m_instance = NULL;

KeyMagicLogger::KeyMagicLogger() {
	m_logFile = stdout;
}

KeyMagicLogger::~KeyMagicLogger() {
	m_instance = NULL;
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
	
	flush();
}

void KeyMagicLogger::flush() {
	fflush(m_logFile);
}

FILE * KeyMagicLogger::getFile() {
	return m_logFile;
}

void KeyMagicLogger::setFile(FILE * file) {
	m_logFile = file;
}
