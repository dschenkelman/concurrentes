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
#include "../Model/SharedCard.h"

using namespace std;

class PlayerHead {

private:
	int number, leftPlayerNumber, rightPlayerNumber;
	vector<Card> hand;

	Semaphore readyToSendReceiveSemaphore,
				receiverSemaphore,
				senderSemaphore,
				receivedSemaphore,
				sentSemaphore;

	SharedCard cardToSendMemory, receivedCardMemory;

public:
	PlayerHead( int playerNumber, int leftPlayerNumber, int rightPlayerNumber);
	virtual ~PlayerHead();

	void playRound();

private:
	Card retrieveCardToSend();
	void informCardHasBeenSelected();
};

#endif /* PLAYERHEAD_H_ */
