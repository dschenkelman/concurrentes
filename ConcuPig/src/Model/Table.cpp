/*
 * Table.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#include "Table.h"
#include "../Services/NamingService.h"
#include "../Constants/SemaphoreNames.h"
#include "../Constants/SignalNumbers.h"
#include "../Model/PigGoesDeck.h"
#include <signal.h>

Table::Table(int numberOfPlayers, std::vector<pid_t>& playerProcesses) :
handDownFifo(NamingService::getHandDownFifoName()),
numberOfPlayers(numberOfPlayers),
playerProcesses(playerProcesses) {
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
				this->notifyRoundOver(playerId);
			}
		}
	} while(!this->scoreboard.trackLost(playerId));

	// game is over
}

void Table::deal(){
	PigGoesDeck deck(this->numberOfPlayers);
	deck.shuffle();

	int i = 0;

	while (deck.hasNextCard()){
		Card card = deck.hit();
		char c[2];
		card.serialize(c);
		this->dealingFifos[i % this->numberOfPlayers].writeValue(c, sizeof(char) * 2);
		i++;
	}
}

void Table::unblockPlayers(void){
	for (unsigned int i = 0; i < this->dealtSemaphores.size(); i++) {
		this->dealtSemaphores[i].signal();
	}
}

void Table::notifyRoundOver(int winner){
	for (int i = 0; i < this->playerProcesses.size(); ++i) {
		if (i != winner){
			kill(this->playerProcesses[i], SignalNumbers::PlayerWon);
		}
	}
}

Table::~Table() {
	for (unsigned int i = 0; i < this->dealingFifos.size(); i++) {
		this->dealingFifos[i].closeFifo();
		this->dealingFifos[i].eliminate();
	}
}
