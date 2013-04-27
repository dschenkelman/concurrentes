/*
 * SharedScoreboard.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#include "SharedScoreboard.h"
#include "../Services/NamingService.h"
#include "../Concurrency/Lock.h"

SharedScoreboard::SharedScoreboard(){
	Lock l(NamingService::getScoreboardFileName());
	this->scoreboardMemory.create(NamingService::getScoreboardKey());
}

void SharedScoreboard::print(){
	Lock l(NamingService::getScoreboardFileName());

	this->scoreboardMemory.getValue().print();
}

bool SharedScoreboard::trackLost(int playerId){
	Lock l(NamingService::getScoreboardFileName());

	return this->scoreboardMemory.getValue().trackLost(playerId);
}

SharedScoreboard::~SharedScoreboard() {
	Lock l(NamingService::getScoreboardFileName());
	this->scoreboardMemory.release();
}
