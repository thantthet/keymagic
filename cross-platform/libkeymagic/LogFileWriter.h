/*
 * LogFileWriter.h
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

#ifndef LOGFILEWRITER_H_
#define LOGFILEWRITER_H_

#include <iostream>

namespace libkm {

	/**
	 * LogFileWriter
	 */
	class LogFileWriter {
	public:
		LogFileWriter();
		LogFileWriter(FILE * file);
		/**
		 * Log to file
		 * @param fmt Format
		 */
		void log(const std::string fmt, ...);
		/**
		 * Get current destination file
		 */
		FILE * getFile();
		/**
		 * Set file to output
		 */
		void setFile(FILE * file);
		/**
		 * Flush
		 */
		void flush();
		
		unsigned int indentation;
	private:
		FILE * m_logFile;
	};

}

#endif