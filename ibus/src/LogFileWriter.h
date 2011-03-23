/*
 * LogFileWriter.h
 *
 *  Created on: March 20, 2011
 *      Author: thantthetkz
 */

#ifndef LOGFILEWRITER_H_
#define LOGFILEWRITER_H_

#include <stdio.h>
#include <iostream>

using namespace std;

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
	void log(const char * fmt, ...);
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
private:
	FILE * m_logFile;
};

#endif
