/*
 * ConcurrencyManager.h
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#ifndef CONCURRENCYMANAGER_H_
#define CONCURRENCYMANAGER_H_

#include <string>
#include <sys/types.h>
#include <vector>
#include "../Concurrency/Semaphore.h"
#include "../Concurrency/Fifo.h"
#include "../Model/SharedScoreboard.h"

class ConcurrencyManager {
private:
	int players;
	std::vector<Semaphore> semaphores;
	std::vector<Fifo> fifos;
	SharedScoreboard* scoreboard;
	void initializeSharedMemories(void);
	void initializeSemaphores(void);
	void initializeFifos(void);
	void initializeSemaphore(key_t key, int initialValue);
	void openFifo(const std::string& name);
	void terminateSemaphores(key_t key, int initialValue);
	void createFile(const std::string& fileName);
public:
	ConcurrencyManager(int players);
	void initialize(void);
	void terminate(void);
	virtual ~ConcurrencyManager();
};

#endif /* CONCURRENCYMANAGER_H_ */
