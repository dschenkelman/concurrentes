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
		static Logger *singletonLogger;
		static bool instanceFlag;
		char cCurrentPath[FILENAME_MAX];
		std::string logFile;
		std::ofstream file;
		time_t rawtime;

		Logger();

	public:
		static Logger* getInstance();
		void logLine(std::string &logLine,LoggerLevels logLevel);
		~Logger();
};


#endif /* LOGGER_H_ */
