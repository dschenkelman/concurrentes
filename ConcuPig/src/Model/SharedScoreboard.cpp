/*
 * SharedScoreboard.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#include "SharedScoreboard.h"
#include "../Services/NamingService.h"
#include "../Concurrency/Lock.h"
#include "../Helpers/Convert.h"

#include <iostream>
using namespace std;

SharedScoreboard::SharedScoreboard(int players, bool initializeValues){
	this->players = players;
	for (int i = 0; i < players; i++) {
		Score* s = new Score(i, initializeValues);
		int score = s->getScore();
		cout << "Player " << Convert::ToString(i) << " score: " << Convert::ToString(score) << endl;
		this->playerScores.push_back(s);
	}
}

void SharedScoreboard::print(){
	for (int i = 0; i < this->players; i++) {
		int score = this->playerScores[i]->getScore();
		cout << "Player " << Convert::ToString(i) << " score: " << Convert::ToString(score) << endl;
	}
}

bool SharedScoreboard::trackLost(int playerId){
	return this->playerScores[playerId]->trackLost();
}

SharedScoreboard::~SharedScoreboard() {
	for (int i = 0; i < this->playerScores.size(); i++) {
		delete this->playerScores[i];
	}
}
