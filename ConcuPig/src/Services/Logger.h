#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "LoggerLevels.h"
#include "../Concurrency/Lock.h"
#include "../Helpers/Convert.h"

class Logger {

	private:
		Logger();
		static Logger *singletonLogger;
		static bool enabled;
		std::string logFile;
		std::ofstream file;
		time_t rawtime;

	public:
		static void enable();
		static Logger* getInstance();
		static void terminate();
		void logLine(const std::string &logLine,LoggerLevels logLevel);
		void logLine(const char* logLine, LoggerLevels logLevel);
		void logPlayer(int playerNumber,const std::string &logLine,LoggerLevels logLevel);
		~Logger();
};


#endif /* LOGGER_H_ */
