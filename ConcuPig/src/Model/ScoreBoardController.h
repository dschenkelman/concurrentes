/*
 * ScoreBoardController.h
 *
 *  Created on: 27/04/2013
 *      Author: matias
 */

#ifndef SCOREBOARDCONTROLLER_H_
#define SCOREBOARDCONTROLLER_H_

class ScoreBoardController {

private:
	SharedScoreboard scoreBoardMemory;

public:
	ScoreBoardController();
	virtual ~ScoreBoardController();

	void run();
};

#endif /* SCOREBOARDCONTROLLER_H_ */
