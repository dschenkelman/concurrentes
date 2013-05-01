#ifndef PLAYERHEAD_H_
#define PLAYERHEAD_H_

#include <vector>

#include "../Model/Card.h"
#include "../Concurrency/Semaphore.h"
#include "../Model/SharedCard.h"
#include "../Concurrency/Fifo.h"
#include "../Concurrency/IEventHandler.h"
#include <sys/types.h>
#include <signal.h>

class PlayerHead : public IEventHandler {

private:
	sig_atomic_t playingRound;
	sig_atomic_t gameOver;
	pid_t receiverProcessId;
	pid_t senderProcessId;
	int number;
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
	void logHand();
public:
	PlayerHead( int playerNumber, pid_t receiverProcess, pid_t senderProcess);
	virtual ~PlayerHead();
	void run();
	bool isWinningHand();
	void takeCard(Card card);
	int handleSignal(int signum);
};

#endif /* PLAYERHEAD_H_ */
