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
#include <string.h>

#include "LogFileWriter.h"

namespace libkm {

	LogFileWriter::LogFileWriter() : m_logFile(stdout), indentation(0)  {
	}

	LogFileWriter::LogFileWriter (FILE * file) : indentation(0) {
		m_logFile = file;
	}

	void LogFileWriter::log(const std::string fmt, ...) {
		va_list vl;
		
		if (indentation) { // print out indentation if any
			int spaces = 3 * indentation;
			std::string str(spaces, ' ');
			fprintf(m_logFile, "%s", str.c_str());
		}

		va_start(vl, fmt);
		vfprintf(m_logFile, fmt.c_str(), vl);
		va_end(vl);
		this->flush();
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