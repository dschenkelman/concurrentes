#ifndef PLAYERSYNCHRONIZER_H_
#define PLAYERSYNCHRONIZER_H_

#include "../Concurrency/Fifo.h"
#include "../Concurrency/Semaphore.h"
#include <vector>
#include <signal.h>
#include "../Concurrency/IEventHandler.h"

class PlayerSynchronizer : public IEventHandler {

	private:
		sig_atomic_t gameOver;
		Fifo playersReadyFifo;
		Fifo playersEverybodyPickUpCardFifo;
		int numberOfPlayers;
		std::vector<Semaphore> playersReadySemaphores;
		std::vector<Semaphore> playersEverybodyPickUpCardSemaphores;

		void unblockPlayersReady(void);
		void unblockPlayersFinished(void);

	public:
		PlayerSynchronizer(int numberOfPlayers);
		void run(void);
		~PlayerSynchronizer();
		int handleSignal(int signum);
};


#endif /* PLAYERSYNCHRONIZER_H_ */
