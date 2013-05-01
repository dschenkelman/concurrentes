#ifndef SCORE_H_
#define SCORE_H_

#include "../Concurrency/SharedMemory.h"
#include <string>

class Score {

private:
	SharedMemory<int> sharedScore;
	std::string fileName;

public:

	Score (int playerNumber, bool initialieValue);
	virtual ~Score();

	void setScore(int score);
	int getScore();

	bool trackLost();
};


#endif /* SCORE_H_ */
