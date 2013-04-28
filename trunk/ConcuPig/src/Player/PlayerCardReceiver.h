#ifndef PLAYERCARDRECEIVER_H_
#define PLAYERCARDRECEIVER_H_

#include "../Concurrency/Semaphore.h"
#include "../Concurrency/Fifo.h"
#include "../Model/SharedCard.h"
#include <string>

class PlayerCardReceiver{

	private:
		Semaphore receiverSemaphore,receivedSemaphore;
		SharedCard sharedCard;
		Fifo fifo;
		int playerNumber,playerOrigin;
		std::string fifoName;

	public:
		PlayerCardReceiver(int playerNumber, int playerTaget);
		~PlayerCardReceiver();
		void run();

};


#endif /* PLAYERCARDRECEIVER_H_ */
