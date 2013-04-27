#include "Logger.h"

#include <unistd.h>

using namespace std;

bool Logger::instanceFlag = false;
Logger* Logger::singletonLogger = NULL;

Logger::Logger(){

	getcwd(cCurrentPath, sizeof(cCurrentPath));
	this->logFile = string(cCurrentPath)+"/logFileTMP";

	this->file.open (this->logFile.c_str(), ios::out | ios::app);

	time (&this->rawtime);
	this->file << ctime (&rawtime) << "STARTING LOGGING" << endl << flush;
}

Logger :: ~Logger (){
    this->instanceFlag = false;
    this->file.close();
}

Logger* Logger::getInstance()
{
    if(! instanceFlag)
    {
        singletonLogger = new Logger();
        instanceFlag = true;
        return singletonLogger;
    }
    else
    {
        return singletonLogger;
    }
}

void Logger::logLine(string &logLine,LoggerLevels logLevel)
{
	time (&this->rawtime);

	{
		Lock l(this->logFile);
		this->file << "Log Level: "<< logLevel <<" - "<< logLine.c_str() <<" - "<< ctime(&this->rawtime) << endl << flush;
	}

}

