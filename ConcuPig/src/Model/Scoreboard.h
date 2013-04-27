/*
 * Scoreboard.h
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#ifndef SCOREBOARD_H_
#define SCOREBOARD_H_

#include <vector>

class Scoreboard {
private:
	std::vector<int> scores;
public:
	Scoreboard(int players);
	// returns true if the game is over, false otherwise
	bool trackLost(int playerId);
	void print(void);
	virtual ~Scoreboard();
};

#endif /* SCOREBOARD_H_ */
