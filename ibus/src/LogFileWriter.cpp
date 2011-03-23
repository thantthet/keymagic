/*
 * LogFileWriter.cpp
 *
 *  Created on: March 20, 2011
 *      Author: thantthetkz
 */

#include <stdio.h>
#include <stdarg.h>

#include "LogFileWriter.h"

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