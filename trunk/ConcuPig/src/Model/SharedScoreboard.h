/*
 * SharedScoreboard.h
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#ifndef SHAREDSCOREBOARD_H_
#define SHAREDSCOREBOARD_H_

#include "../Concurrency/SharedMemory.h"
#include "../Model/Scoreboard.h"
#include "../Model/Score.h"
#include <vector>

class SharedScoreboard {
	std::vector<Score*> playerScores;
	int players;

public:
	SharedScoreboard(int players, bool initializeValues);
	void print();
	bool trackLost(int playerId);
	virtual ~SharedScoreboard();
};

#endif /* SHAREDSCOREBOARD_H_ */
