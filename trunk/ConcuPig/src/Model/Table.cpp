/*
 * Table.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#include "Table.h"
#include "../Services/NamingService.h"
#include "../Constants/SemaphoreNames.h"

Table::Table(int numberOfPlayers) : handDownFifo(NamingService::getHandDownFifoName()), numberOfPlayers(numberOfPlayers) {
	for (int i = 0; i < numberOfPlayers; ++i) {
		Fifo f(NamingService::getDealingFifoName(i));
		this->dealingFifos.push_back(f);
		Semaphore s(NamingService::getSemaphoreKey(SemaphoreNames::DealtSemaphore, i));
		this->dealtSemaphores.push_back(s);
	}
}

void Table::run(){
	int playerId = -1;

	do {
		this->deal();
		this->unblockPlayers();
		int handPutDown = 0;
		while(handPutDown != this->numberOfPlayers){

			this->handDownFifo.readValue((char*)&playerId, sizeof(int));
			handPutDown++;

			if (handPutDown == 1){
				this->notifyRoundOver();
			}
		}
	} while(!this->scoreboard.trackLost(playerId));

	// game is over
}

void Table::deal(){

}

void Table::unblockPlayers(void){
	for (unsigned int i = 0; i < this->dealtSemaphores.size(); i++) {
		this->dealtSemaphores[i].signal();
	}
}

void Table::notifyRoundOver(void){

}

Table::~Table() {
	// TODO Auto-generated destructor stub
}

