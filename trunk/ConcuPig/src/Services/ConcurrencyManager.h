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

class ConcurrencyManager {
private:
	int players;
	std::vector<Semaphore> semaphores;
	void initializeSharedMemories(void);
	void initializeSemaphores(void);
	void initializeSemaphore(key_t key, int initialValue);
	void terminateSemaphores(key_t key, int initialValue);
	void createFile(const std::string& fileName);
public:
	ConcurrencyManager(int players);
	void initialize(void);
	void terminate(void);
	virtual ~ConcurrencyManager();
};

#endif /* CONCURRENCYMANAGER_H_ */
