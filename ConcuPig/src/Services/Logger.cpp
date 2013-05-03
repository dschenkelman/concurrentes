#include "Logger.h"

#include <unistd.h>

#define	logFileName	"logFileTMP"

using namespace std;

Logger* Logger::singletonLogger = NULL;

Logger::Logger(){

	this->logFile = logFileName;
	this->file.open (this->logFile.c_str(), ios::out | ios::app);

	time (&this->rawtime);
	this->file << ctime (&rawtime) << "STARTING LOGGING" << endl << flush;
}

Logger :: ~Logger (){
    this->file.close();
}

Logger* Logger::getInstance()
{
	if (singletonLogger == NULL)
		singletonLogger = new Logger();

	return singletonLogger;
}

void Logger::terminate()
{
	if ( singletonLogger == NULL )
		delete singletonLogger;
}

void Logger::logPlayer(int playerNumber,const string &logLine,LoggerLevels logLevel){
	string newLogLine = "(Player: "+Convert::toString(playerNumber)+") "+logLine;
	this->logLine(newLogLine,logLevel);
}

void Logger::logLine(const string &logLine,LoggerLevels logLevel)
{
	time (&this->rawtime);

	Lock l(this->logFile);
	this->file << "PId: " << getpid() << " - Log Level: "<< logLevel <<" - "<< logLine.c_str() <<" - "<< ctime(&this->rawtime) << flush;
}

void Logger::logLine(const char* logLine, LoggerLevels logLevel){
	time (&this->rawtime);

	Lock l(this->logFile);
	this->file << "PId: " << getpid() << " - Log Level: "<< logLevel <<" - "<< logLine <<" - "<< ctime(&this->rawtime) << flush;
}
