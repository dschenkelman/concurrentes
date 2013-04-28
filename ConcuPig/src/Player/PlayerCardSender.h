#ifndef PLAYERCARDSENDER_H_
#define PLAYERCARDSENDER_H_

#include "../Concurrency/Semaphore.h"
#include "../Concurrency/Fifo.h"
#include "../Model/SharedCard.h"
#include "../Services/Logger.h"
#include "../Helpers/Convert.h"
#include <string>

class PlayerCardSender{

	private:
		Semaphore senderSemaphore,sentSemaphore;
		SharedCard sharedCard;
		Fifo fifo;
		std::string fifoName;
		int playerNumber,playerTarget;

	public:
		PlayerCardSender(int playerNumber, int playerTaget);
		~PlayerCardSender();
		void run();

};

#endif /* PLAYERCARDSENDER_H_ */
