/*
 * Table.h
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#ifndef TABLE_H_
#define TABLE_H_

#include "../Concurrency/Fifo.h"
#include "../Concurrency/Semaphore.h"
#include <vector>
#include "../Model/Scoreboard.h"
#include "PlayerSynchronizer.h"
#include <sys/types.h>

class Table {
private:
	Fifo handDownFifo;
	Fifo playerWonFifo;
	int numberOfPlayers;
	std::vector<Fifo> dealingFifos;
	std::vector<Semaphore> dealtSemaphores;
	std::vector<pid_t> playerProcesses;
	pid_t syncProcessId;
	Scoreboard scoreboard;

	void deal(void);
	void notifyRoundOver(int winner);
	void notifyGameOver(void);
	void unblockPlayers(void);
	int checkForLoser(void);
public:
	Table(int numberOfPlayers, int syncProcess, std::vector<int>& playerProcesses);
	void run(void);
	virtual ~Table();
};

#endif /* TABLE_H_ */
