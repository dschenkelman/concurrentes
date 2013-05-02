/*
 * Scoreboard.h
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#ifndef SCOREBOARD_H_
#define SCOREBOARD_H_

#include "../Concurrency/SharedMemory.h"
#include "../Model/Score.h"
#include <vector>

class Scoreboard {
	std::vector<Score*> playerScores;
	int players;
public:
	Scoreboard(int players, bool initializeValues);
	void print();
	bool trackLost(int playerId);
	virtual ~Scoreboard();
};

#endif /* SCOREBOARD_H_ */
