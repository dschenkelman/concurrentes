/*
 * Logger.cpp
 *
 *  Created on: 21/06/2013
 *      Author: matias
 */

#include "Logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Logger* Logger::instance = NULL;

using namespace std;

Logger::Logger()
{
	loggingEnable = true;

	time (&rawtime);
	name = "ServerSession : " + std::string(ctime(&rawtime));
	file.open (name.c_str(), ios::out | ios::app);
	file 	<< ctime(&rawtime)
			<< "STARTING LOGGING"
			<< endl << flush;
}

Logger::~Logger()
{
	file.close();
}

void Logger::terminate()
{
	if ( NULL == Logger::instance )
	{
		delete Logger::instance;
	}
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

	time(&rawtime);
	file 	<< ctime(&this->rawtime) << " : "
			<< message << flush;
}

void Logger::logRequest(struct messageRequest request)
{
	if( false == loggingEnable )
		return;

	time(&rawtime);
	std::string registerString = "";
	registerString += "\nClient Id : " + request.clientId;
	registerString += "\nAction type : " + request.requestActionType;
	registerString += "\nPerson name : " + std::string(request.name);
	registerString += "\nPerson address : " + std::string(request.address);
	registerString += "\nPerson phone : " + std::string(request.telephone);
	registerString += "\nPerson name Id : " + std::string(request.nameId);

	file 	<< ctime(&this->rawtime) << " : "
			<< registerString << flush;
}

