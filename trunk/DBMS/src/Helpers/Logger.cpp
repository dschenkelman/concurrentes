/*
 * Logger.cpp
 *
 *  Created on: 21/06/2013
 *      Author: matias
 */

#include "Logger.h"

#include "Convert.h"
#include "../Messages/MessageActions.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Logger* Logger::instance = NULL;

using namespace std;

std::string getActionTypeName(int id);

Logger::Logger()
{
	loggingEnable = true;

	time (&rawtime);
	name = "ServerSession : " + std::string(ctime(&rawtime)) + ".txt";
	file.open (name.c_str(), ios::out | ios::app);
	file 	<< ctime(&rawtime)
			<< "STARTING LOGGING"
			<< endl << endl << flush;
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
	file 	<< ctime(&this->rawtime)
			<< message << endl << endl << flush;
}

void Logger::logRequest(struct messageRequest request)
{
	if( false == loggingEnable )
		return;

	time(&rawtime);
	std::string registerString = "";
	registerString += "\nClient Id : " + Convert::toString(request.clientId);
	registerString += "\nAction type id : " + Convert::toString(request.requestActionType);

	registerString += "\nAction type name : ";
	registerString += getActionTypeName(request.requestActionType);

	registerString += "\nPerson name : " + std::string(request.name);
	registerString += "\nPerson address : " + std::string(request.address);
	registerString += "\nPerson phone : " + std::string(request.telephone);
	registerString += "\nPerson name Id : " + std::string(request.nameId);

	file 	<< ctime(&this->rawtime)
			<< registerString << endl << endl << flush;
}

std::string getActionTypeName(int id)
{
	switch(id)
	{
	case CREATE:
	{
		return "CREATE";
	}
	case UPDATE:
	{
		return "UPDATE";
	}
	case READ:
	{
		return "READ";
	}
	case DELETE:
	{
		return "DELETE";
	}
	case HEAD:
	{
		return "HEAD";
	}
	case OPERATION_FAILED:
	{
		return "OPERATION_FAILED";
	}
	case ENDOFCONNECTION:
	{
		return "ENDOFCONNECTION";
	}
	case OPERATION_CREATE_SUCCESS:
	{
		return "OPERATION_CREATE_SUCCESS";
	}
	case OPERATION_UPDATE_SUCCESS:
	{
		return "OPERATION_UPDATE_SUCCESS";
	}
	case OPERATION_DELETE_SUCCESS:
	{
		return "OPERATION_DELETE_SUCCESS";
	}
	case OPERATION_UNKNOWN:
	{
		return "OPERATION_UNKNOWN";
	}
	case BODY:
	{
		return "BODY";
	}
	case GRACEFUL_QUIT:
	{
		return "GRACEFUL_QUIT";
	}
	case NULL_ACTION_TYPE:
	{
		return "NULL_ACTION_TYPE";
	}
	}
	return "";
}
