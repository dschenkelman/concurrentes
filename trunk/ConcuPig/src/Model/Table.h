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
#include "../Model/SharedScoreboard.h"
#include "PlayerSynchronizer.h"
#include <sys/types.h>

class Table {
private:
	Fifo handDownFifo;
	int numberOfPlayers;
	std::vector<Fifo> dealingFifos;
	std::vector<Semaphore> dealtSemaphores;
	std::vector<pid_t> playerProcesses;
	pid_t syncProcessId;
	SharedScoreboard scoreboard;

	void deal(void);
	void notifyRoundOver(int winner);
	void notifyGameOver();
	void unblockPlayers(void);
public:
	Table(int numberOfPlayers, int syncProcess, std::vector<int>& playerProcesses);
	void run(void);
	virtual ~Table();
};

#endif /* TABLE_H_ */
