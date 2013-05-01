/*
 * NamingService.h
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#ifndef NAMINGSERVICE_H_
#define NAMINGSERVICE_H_

#include <string>
#include <sys/ipc.h>

class NamingService {
public:
	static key_t getSemaphoreKey(const std::string& kind, int playerNumber);
	static std::string getCardPassingFifoFileName(int originPlayer, int targetPlayer);
	static std::string getDealingFifoName(int player);
	static std::string getHandDownFifoName();
	static std::string getPlayersReadyFifoName();
	static key_t getSharedCardKey(const std::string& cardType, int playerNumber);
	static std::string getSharedCardFileName(const std::string& cardType, int playerNumber);
	static key_t getScoreboardKey();
	static std::string getScoreboardFileName();
	static key_t getSharedScoreKey(int player);
	static std::string getSharedScoreFileName(int player);
};

#endif /* NAMINGSERVICE_H_ */
