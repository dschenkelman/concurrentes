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

class Table {
private:
	Fifo handDownFifo;
	int numberOfPlayers;
	std::vector<Fifo> dealingFifos;
	std::vector<Semaphore> dealtSemaphores;
	SharedScoreboard scoreboard;

	void deal(void);
	void notifyRoundOver(void);
	void unblockPlayers(void);
public:
	Table(int numberOfPlayers);
	void run(void);
	virtual ~Table();
};

#endif /* TABLE_H_ */
