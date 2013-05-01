#ifndef PLAYERSYNCHRONIZER_H_
#define PLAYERSYNCHRONIZER_H_

#include "../Concurrency/Fifo.h"
#include "../Concurrency/Semaphore.h"
#include <vector>
#include "../Concurrency/IEventHandler.h"

class PlayerSynchronizer : public IEventHandler {

	private:
		bool gameOver;
		Fifo playersReadyFifo;
		int numberOfPlayers;
		std::vector<Semaphore> playersReadySemaphores;

		void unblockPlayers(void);

	public:
		PlayerSynchronizer(int numberOfPlayers);
		void run(void);
		~PlayerSynchronizer();
		int handleSignal(int signum);
};


#endif /* PLAYERSYNCHRONIZER_H_ */
