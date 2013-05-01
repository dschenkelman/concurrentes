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
#include "../Services/Logger.h"
#include "../Helpers/Convert.h"
#include <signal.h>
#include <string>

using namespace std;

Table::Table(int numberOfPlayers, int syncProcess, std::vector<pid_t>& playerProcesses) :
handDownFifo(NamingService::getHandDownFifoName()),
numberOfPlayers(numberOfPlayers),
playerProcesses(playerProcesses),
syncProcessId(syncProcess){
	for (int i = 0; i < numberOfPlayers; ++i) {
		Fifo f(NamingService::getDealingFifoName(i));
		this->dealingFifos.push_back(f);
		Semaphore s(NamingService::getSemaphoreKey(SemaphoreNames::DealtSemaphore, i));
		this->dealtSemaphores.push_back(s);
	}
}

void Table::run(){
	int playerId = -1;

	string message = "Table starting";
	Logger::getInstance()-> logLine(message, INFO);

	do {
		this->deal();
		this->unblockPlayers();
		int handPutDown = 0;
		while(handPutDown != this->numberOfPlayers){
			message = "Waiting for player to finish";
			Logger::getInstance()-> logLine(message, INFO);

			char id[4];
			this->handDownFifo.readValue(id, sizeof(char) * 4);
			playerId = id[0] + (id[1] << 8) + (id[2] << 16) + (id[3] << 24);

			handPutDown++;
			message = "Player put hand down: " + Convert::ToString(playerId) + ". Total: " + Convert::ToString(handPutDown);
			Logger::getInstance()-> logLine(message, INFO);

			if (handPutDown == 1){
				this->notifyRoundOver(playerId);
			}
		}

		message = "Table - Round Finished.  Player that lost: " + Convert::ToString(playerId);
		Logger::getInstance()-> logLine(message, INFO);
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
		string m = "Dealing card " + Convert::ToString(i);
		Logger::getInstance()-> logLine(m, INFO);
		int result = this->dealingFifos[i % this->numberOfPlayers].writeValue(c, sizeof(char) * 2);
		m = "Dealing FIFO result " + Convert::ToString(result);
		Logger::getInstance()-> logLine(m, INFO);
		i++;
	}

	Logger::getInstance()-> logLine("Finished dealing", INFO);
}

void Table::unblockPlayers(void){
	for (unsigned int i = 0; i < this->dealtSemaphores.size(); i++) {
		string s = "Signaling semaphore " + Convert::ToString(i);
		Logger::getInstance()-> logLine(s, INFO);
		this->dealtSemaphores[i].signal();
	}
}

void Table::notifyRoundOver(int winner){
	for (int i = 0; i < this->playerProcesses.size(); ++i) {
		if (i != winner){
			string message = "Notifying round over to player - " + Convert::ToString(i);
			Logger::getInstance()-> logLine(message, INFO);
			kill(this->playerProcesses[i], SignalNumbers::PlayerWon);
		}
	}
}

void Table::notifyGameOver(){
	for (int i = 0; i < this->playerProcesses.size(); ++i) {
		string message = "Notifying  game over to player - " + Convert::ToString(i);
		Logger::getInstance()-> logLine(message, INFO);
		kill(this->playerProcesses[i], SignalNumbers::GameOver);
	}

	kill(this->syncProcessId, SignalNumbers::GameOver);
}

Table::~Table() {
	for (unsigned int i = 0; i < this->dealingFifos.size(); i++) {
		this->dealingFifos[i].closeFifo();
	}
}
