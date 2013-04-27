/*
 * PlayerHead.h
 *
 *  Created on: 27/04/2013
 *      Author: matias
 */

#ifndef PLAYERHEAD_H_
#define PLAYERHEAD_H_

#include <vector>

#include "../Model/Card.h"
#include "../Concurrency/Semaphore.h"

using namespace std;

class PlayerHead {

private:
	int position, leftPlayerPosition, rightPlayerPosition;
	vector<Card> hand;

	Semaphore readyToSendReceiveSemaphore;

public:
	PlayerHead( int position, int leftPlayerPosition, int rightPlayerPosition);
	virtual ~PlayerHead();

	void playRound();

private:
	Card retrieveCardToSend();
	void informCardHasBeenSelected();
};

#endif /* PLAYERHEAD_H_ */
