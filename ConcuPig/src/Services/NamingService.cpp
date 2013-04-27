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

key_t NamingService::getSemaphoreKey(const std::string& kind, int playerNumber){
	return ftok(kind.c_str(), playerNumber);
}

string NamingService::getCardPassingFifoFileName(int originPlayer, int targetPlayer){
	return FifoNames::CardPassing + "_" + Convert::ToString(originPlayer) + "_" + Convert::ToString(targetPlayer);
}

string NamingService::getDealingFifoName(int player){
	return FifoNames::Dealing + Convert::ToString(player);
}

string NamingService::getHandDownFifoName(){
	return FifoNames::HandDown;
}

key_t NamingService::getSharedCardKey(const std::string& cardType, int playerNumber){
	return ftok(getSharedCardFileName(cardType, playerNumber).c_str(), 'a');
}

string NamingService::getSharedCardFileName(const std::string& cardType, int playerNumber){
	string fileName = cardType.c_str() + Convert::ToString(playerNumber);
	return fileName;
}

key_t NamingService::getScoreboardKey(){
	string fileName = getScoreboardFileName();
	return ftok(fileName.c_str(), 'a');
}

string NamingService::getScoreboardFileName(){
	return SharedMemoryNames::Scoreboard;
}
