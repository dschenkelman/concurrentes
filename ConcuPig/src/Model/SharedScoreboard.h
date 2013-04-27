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

class SharedScoreboard {
	SharedMemory<Scoreboard> scoreboardMemory;
public:
	SharedScoreboard();
	void print();
	bool trackLost(int playerId);
	virtual ~SharedScoreboard();
};

#endif /* SHAREDSCOREBOARD_H_ */
