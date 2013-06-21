/*
 * Logger.h
 *
 *  Created on: 21/06/2013
 *      Author: matias
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <list>
#include <stdio.h>

#include "../Messages/MessageRequest.h"

class Logger
{
public:
	static Logger *getInstance();
	static void terminate();
	virtual ~Logger();

	void setLogEnable(bool enable);
	bool isLogEnable();

	void logMessage(std::string message);
	void logRequest(struct messageRequest request);

protected:
	Logger();

private:
	static Logger *instance;

	bool loggingEnable;
	time_t rawtime;
	std::ofstream file;
	std::string name;
};

#endif /* LOGGER_H_ */
