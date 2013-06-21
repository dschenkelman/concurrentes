/*
 * Logger.cpp
 *
 *  Created on: 21/06/2013
 *      Author: matias
 */

#include "Logger.h"

Logger::Logger()
{
	loggingEnable = true;
}

Logger::~Logger()
{
}


Logger *Logger::getInstance()
{
	if( NULL == Logger::instance )
	{
		Logger::instance = new Logger();
	}
	return Logger::instance;
}

void Logger::setLogEnable(bool enable)
{
	loggingEnable = enable;
}

bool Logger::isLogEnable()
{
	return loggingEnable;
}

void Logger::logMessage(std::string message)
{
	if( false == loggingEnable )
		return;

	// log the message
}

void Logger::logRequest(struct messageRequest request)
{
	if( false == loggingEnable )
		return;

	// log the register
}

