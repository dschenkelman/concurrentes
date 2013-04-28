#ifndef PLAYERSYNCHRONIZER_H_
#define PLAYERSYNCHRONIZER_H_

#include "../Concurrency/Fifo.h"
#include "../Concurrency/Semaphore.h"
#include <vector>

class PlayerSynchronizer{

	private:
		Fifo playersReadyFifo;
		int numberOfPlayers;
		std::vector<Semaphore> playersReadySemaphores;

		void unblockPlayers(void);

	public:
		PlayerSynchronizer(int numberOfPlayers);
		void run(void);
		~PlayerSynchronizer();

};


#endif /* PLAYERSYNCHRONIZER_H_ */
