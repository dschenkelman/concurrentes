#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "LoggerLevels.h"
#include "../Concurrency/Lock.h"

class Logger {

	private:
		Logger(void);
		static Logger *singletonLogger;
		std::string logFile;
		std::ofstream file;
		time_t rawtime;

	public:
		static Logger* getInstance();
		void logLine(std::string &logLine,LoggerLevels logLevel);
		~Logger();
};


#endif /* LOGGER_H_ */
