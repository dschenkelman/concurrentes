#ifndef PLAYERCARDSENDER_H_
#define PLAYERCARDSENDER_H_

#include "../Concurrency/Semaphore.h"
#include "../Concurrency/Fifo.h"
#include "../Model/SharedCard.h"

class PlayerCardSender{

	private:
		Semaphore senderSemaphore,sentSemaphore;
		SharedCard sharedCard;
		Fifo fifo;
		int playerNumber,playerTarget;

	public:
		PlayerCardSender(int playerNumber, int playerTaget);
		~PlayerCardSender();
		void run();

};

#endif /* PLAYERCARDSENDER_H_ */
