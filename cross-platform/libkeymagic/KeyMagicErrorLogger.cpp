/*
 * KeyMagicErrorLogger.cpp
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

#include "KeyMagicErrorLogger.h"

namespace libkm {

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

}