/*
 * NamingService.h
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#ifndef NAMINGSERVICE_H_
#define NAMINGSERVICE_H_

#include <string>

class NamingService {
public:
	static key_t getSemaphoreName(const std::string& kind, int playerNumber);
	static key_t getCardPassingFifo(int originPlayer, int targetPlayer);
	static key_t getDealingFifo(int player);
	static key_t getHandDownFifo();
	static key_t getSharedCard(const std::string& cardType, int playerNumber);
	static key_t getScorboard();
};

#endif /* NAMINGSERVICE_H_ */
