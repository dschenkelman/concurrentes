/*
 * NamingService.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#include "NamingService.h"
#include <string>
#include "../Constants/FifoNames.h"
#include "../Constants/SharedMemoryNames.h"
#include "../Helpers/Convert.h"

using namespace std;

key_t NamingService::getSemaphoreName(const std::string& kind, int playerNumber){
	return ftok(kind.c_str(), playerNumber);
}

key_t NamingService::getCardPassingFifo(int originPlayer, int targetPlayer){
	string fileName = FifoNames::CardPassing + "_" + Convert::ToString(originPlayer) + "_" + Convert::ToString(targetPlayer);
	return ftok(fileName.c_str(), 'a');
}

key_t NamingService::getDealingFifo(int player){
	return ftok(FifoNames::Dealing.c_str(), player);
}

key_t NamingService::getHandDownFifo(){
	return ftok(FifoNames::HandDown.c_str(), 'a');
}

key_t NamingService::getSharedCard(const std::string& cardType, int playerNumber){
	return ftok(cardType.c_str(), playerNumber);
}

key_t NamingService::getScorboard(){
	return ftok(SharedMemoryNames::Scoreboard.c_str(), 'a');
}
