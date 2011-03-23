/*
 * KeyMagicErrorLogger.cpp
 *
 *  Created on: March 20, 2011
 *      Author: thantthetkz
 */

#include "KeyMagicErrorLogger.h"

KeyMagicErrorLogger* KeyMagicErrorLogger::m_instance = NULL;

KeyMagicErrorLogger * KeyMagicErrorLogger::getInstance() {
	if (m_instance == NULL) {
		m_instance = new KeyMagicErrorLogger();
	}
	return m_instance;
}

KeyMagicErrorLogger::KeyMagicErrorLogger() : LogFileWriter(stderr) {
}

KeyMagicErrorLogger::~KeyMagicErrorLogger() {
	m_instance = NULL;
}