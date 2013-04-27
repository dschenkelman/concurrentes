/*
 * Scoreboard.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#include "Scoreboard.h"
#include <iostream>
#include "../Helpers/Convert.h"

using namespace std;

Scoreboard::Scoreboard(int players) {
	for (int i = 0; i < players; i++) {
		this->scores.push_back(0);
	}
}

bool Scoreboard::trackLost(int playerId){
	this->scores[playerId]++;

	return this->scores[playerId] == 7;
}

void Scoreboard::print(){
	for (unsigned int i = 0; i < this->scores.size(); i++) {
		cout << "Player " << Convert::ToString(i) << " score: " << Convert::ToString(this->scores[i]) << endl;
	}
}

Scoreboard::~Scoreboard() {
}
