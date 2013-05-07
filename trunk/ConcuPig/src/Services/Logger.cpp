#include "Logger.h"

#include <unistd.h>

#define	logFileName	"LOG"

using namespace std;

Logger* Logger::singletonLogger = NULL;

bool Logger::enabled = false;

const string Logger::logFile = "LOG";

Logger::Logger(){
	if (enabled){

		this->file.open (Logger::logFile.c_str(), ios::out | ios::app);

		time (&this->rawtime);
		this->file << ctime (&rawtime) << "STARTING LOGGING" << endl << flush;
	}
}

Logger :: ~Logger (){
	if (enabled){
		this->file.close();
	}
}

void Logger::enable()
{
	enabled = true;
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
	if (!enabled){
		return;
	}
	string newLogLine = "(Player: "+Convert::toString(playerNumber)+") "+logLine;
	this->logLine(newLogLine,logLevel);
}

void Logger::logLine(const string &logLine,LoggerLevels logLevel)
{
	if (!enabled){
		return;
	}
	time (&this->rawtime);

	Lock l(this->logFile);
	this->file << "PId: " << getpid() << " - Log Level: "<< logLevel <<" - "<< logLine.c_str() <<" - "<< ctime(&this->rawtime) << flush;
}

void Logger::logLine(const char* logLine, LoggerLevels logLevel){
	if (!enabled){
		return;
	}

	time (&this->rawtime);

	Lock l(this->logFile);
	this->file << "PId: " << getpid() << " - Log Level: "<< logLevel <<" - "<< logLine <<" - "<< ctime(&this->rawtime) << flush;
}
