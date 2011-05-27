/*
 * KeyMagicLogger.h
 *
 * Copyright (C) 2008  KeyMagic Project
 * http://keymagic.googlecode.com
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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
