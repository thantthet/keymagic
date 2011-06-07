/*
 * LogFileWriter.cpp
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

#include <stdio.h>
#include <stdarg.h>

#include "LogFileWriter.h"

namespace libkm {

LogFileWriter::LogFileWriter () {
	m_logFile = stdout;
}

LogFileWriter::LogFileWriter (FILE * file) {
	m_logFile = file;
}

void LogFileWriter::log(const char * fmt, ...) {
	va_list vl;
	
	va_start (vl, fmt);
	vfprintf(m_logFile, fmt, vl);
	va_end(vl);
	
	flush();
}

void LogFileWriter::flush() {
	fflush(m_logFile);
}

FILE * LogFileWriter::getFile() {
	return m_logFile;
}

void LogFileWriter::setFile(FILE * file) {
	m_logFile = file;
}
}