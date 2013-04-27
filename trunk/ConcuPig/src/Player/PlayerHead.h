#ifndef PLAYERHEAD_H_
#define PLAYERHEAD_H_

#include <vector>
#include <signal.h>

#include "../Model/Card.h"
#include "../Concurrency/Semaphore.h"
#include "../Model/SharedCard.h"

class PlayerHead {

private:
	pid_t fatherId;
	int number, leftPlayerNumber, rightPlayerNumber;
	std::vector<Card> hand;

	Semaphore readyToSendReceiveSemaphore,
				receiverSemaphore,
				senderSemaphore,
				receivedSemaphore,
				sentSemaphore;

	SharedCard cardToSendMemory, receivedCardMemory;

public:
	PlayerHead( pid_t fatherId,  int playerNumber, int leftPlayerNumber, int rightPlayerNumber);
	virtual ~PlayerHead();

	void playRound();
	bool isWinningHand();
	void takeCard(Card card);

private:
	void createSubProcess();
	void informCardHasBeenSelected();
	void informMyHandIsOnTheTable();
	Card retrieveCardToSend();
};

#endif /* PLAYERHEAD_H_ */
