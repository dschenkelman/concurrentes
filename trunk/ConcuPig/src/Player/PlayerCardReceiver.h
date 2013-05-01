#ifndef PLAYERCARDRECEIVER_H_
#define PLAYERCARDRECEIVER_H_

#include "../Concurrency/Semaphore.h"
#include "../Concurrency/Fifo.h"
#include "../Model/SharedCard.h"
#include <string>
#include "../Concurrency/IEventHandler.h"

class PlayerCardReceiver : public IEventHandler{

	private:
		bool gameOver;
		Semaphore receiverSemaphore,receivedSemaphore;
		Fifo fifo;
		SharedCard sharedCard;
		int playerNumber,playerOrigin;
		std::string fifoName;

	public:
		PlayerCardReceiver(int playerNumber, int playerTaget);
		~PlayerCardReceiver();
		void run();
		int handleSignal(int signum);
};


#endif /* PLAYERCARDRECEIVER_H_ */
