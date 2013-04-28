#ifndef PLAYERHEAD_H_
#define PLAYERHEAD_H_

#include <vector>

#include "../Model/Card.h"
#include "../Concurrency/Semaphore.h"
#include "../Model/SharedCard.h"
#include "../Concurrency/Fifo.h"
#include "../Concurrency/IEventHandler.h"
#include <sys/types.h>

class PlayerHead : public IEventHandler {

private:
	bool playingRound;
	int number, leftPlayerNumber, rightPlayerNumber;
	std::vector<Card> hand;
	Semaphore readyToSendReceiveSemaphore,
				receiverSemaphore,
				senderSemaphore,
				receivedSemaphore,
				sentSemaphore,
				dealtSemaphore;
	SharedCard cardToSendMemory, receivedCardMemory;
	Fifo playerReadyFifo, handDownFifo, dealtFifo;
	void createSubProcess();
	void informCardHasBeenSelected();
	void informMyHandIsOnTheTable();
	Card retrieveCardToSend();
public:
	PlayerHead( int playerNumber, int leftPlayerNumber, int rightPlayerNumber);
	virtual ~PlayerHead();
	void run();
	bool isWinningHand();
	void takeCard(Card card);
	int handleSignal(int signum);
};

#endif /* PLAYERHEAD_H_ */
