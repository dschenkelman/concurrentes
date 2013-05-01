/*
 * ScoreBoardController.h
 *
 *  Created on: 27/04/2013
 *      Author: matias
 */

#ifndef SCOREBOARDCONTROLLER_H_
#define SCOREBOARDCONTROLLER_H_

#include "SharedScoreboard.h"

class ScoreBoardController {

private:
	SharedScoreboard scoreBoardMemory;

public:
	ScoreBoardController(int players);
	virtual ~ScoreBoardController();

	void run();
};

#endif /* SCOREBOARDCONTROLLER_H_ */
