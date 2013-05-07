#include "Score.h"
#include "../Helpers/Convert.h"
#include "../Services/NamingService.h"
#include "../Concurrency/Lock.h"
#include "../Services/Logger.h"
#include <string>

using namespace std;

Score::Score (int playerNumber, bool initializeValue) : playerNumber(playerNumber){
	key_t scoreKey = NamingService::getSharedScoreKey(playerNumber);

	this->fileName = NamingService::getSharedScoreFileName(playerNumber);

	{
		Lock l(this->fileName);
		this->sharedScore.create(scoreKey);
	}

	if (initializeValue){
		this->setScore(0);
	}
}

Score::~Score(){

	{
		Lock l(this->fileName);
		string message = "Score - Destroying";
		Logger::getInstance()->logLine(message, INFO);
		this->sharedScore.release();
	}
	remove(this->fileName.c_str());
}


void Score::setScore(int score){
	Lock l(this->fileName);

	this->sharedScore.setValue(score);
}

int Score::getScore(){
	Lock l(this->fileName);

	return (this->sharedScore.getValue());
}

bool Score::trackLost(){
	{
		Lock l(this->fileName);
		int v = this->sharedScore.getValue();
		v++;
		this->sharedScore.setValue(v);
	}

	int score = this->getScore();
	bool gameOver =  score == 7;

	if (gameOver){
		string message = "Score - Game Over";
		Logger::getInstance()->logLine(message, INFO);
	}
	else{
		string message = "Updated score to " + Convert::toString(score);
		Logger::getInstance()->logLine(message, INFO);
	}

	return gameOver;
}
