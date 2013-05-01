#ifndef PLAYERCARDSENDER_H_
#define PLAYERCARDSENDER_H_

#include "../Concurrency/Semaphore.h"
#include "../Concurrency/Fifo.h"
#include "../Model/SharedCard.h"
#include "../Services/Logger.h"
#include "../Helpers/Convert.h"
#include <string>
#include <signal.h>
#include "../Concurrency/IEventHandler.h"

class PlayerCardSender : public IEventHandler{

	private:
		sig_atomic_t gameOver;
		Semaphore senderSemaphore,sentSemaphore;
		Fifo fifo;
		std::string fifoName;
		int playerNumber,playerTarget;
		SharedCard sharedCard;

	public:
		PlayerCardSender(int playerNumber, int playerTaget);
		~PlayerCardSender();
		void run();
		int handleSignal(int signum);
};

#endif /* PLAYERCARDSENDER_H_ */
