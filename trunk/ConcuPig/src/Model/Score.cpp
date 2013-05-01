#include "Score.h"
#include "../Services/NamingService.h"
#include "../Concurrency/Lock.h"

using namespace std;

Score::Score (int playerNumber){

	key_t scoreKey = NamingService::getSharedScoreKey(playerNumber);

	this->fileName = NamingService::getSharedScoreFileName(playerNumber);

	{
		Lock l(this->fileName);
		this->sharedScore.create(scoreKey);
	}

	this->setScore(0);
}

Score::~Score(){
	Lock l(this->fileName);
	this->sharedScore.release();
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

	int score = this->getScore();
	score += 1;

	this->setScore(score);

	return (score == 7);

}
