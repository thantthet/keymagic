/*
 * KeyMagicLogger.cpp
 *
 *  Created on: Jan 28, 2011
 *      Author: thantthetkz
 */

#include "KeyMagicLogger.h"

KeyMagicLogger* KeyMagicLogger::m_instance = NULL;

KeyMagicLogger * KeyMagicLogger::getInstance() {
	if (m_instance == NULL) {
		m_instance = new KeyMagicLogger();
	}
	return m_instance;
}

KeyMagicLogger::KeyMagicLogger() : LogFileWriter(stderr) {
}

KeyMagicLogger::~KeyMagicLogger() {
	m_instance = NULL;
}
