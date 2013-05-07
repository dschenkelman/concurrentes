/*
 * Scoreboard.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#include "Scoreboard.h"
#include "../Services/NamingService.h"
#include "../Concurrency/Lock.h"
#include "../Helpers/Convert.h"

#include <iostream>
using namespace std;

Scoreboard::Scoreboard(int players, bool initializeValues){
	this->players = players;
	for (int i = 0; i < players; i++) {
		Score* s = new Score(i, initializeValues);
		this->playerScores.push_back(s);
	}
}

void Scoreboard::print(){
	for (int i = 0; i < this->players; i++) {
		int score = this->playerScores[i]->getScore();
		cout << "Player " << Convert::toString(i) << " score: " << Convert::toString(score) << endl;
	}
}

bool Scoreboard::trackLost(int playerId){
	return this->playerScores[playerId]->trackLost();
}

Scoreboard::~Scoreboard() {
	for (unsigned int i = 0; i < this->playerScores.size(); i++) {
		this->playerScores[i]->~Score();
	}
}
