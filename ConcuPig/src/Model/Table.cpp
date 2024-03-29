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
#include <cstring>

using namespace std;

Table::Table(int numberOfPlayers, int syncProcess, std::vector<pid_t>& playerProcesses) :
handDownFifo(NamingService::getHandDownFifoName()),
playerWonFifo(NamingService::getPlayerWonFifoName()),
numberOfPlayers(numberOfPlayers),
playerProcesses(playerProcesses),
syncProcessId(syncProcess),
scoreboard(numberOfPlayers, false){
	for (int i = 0; i < numberOfPlayers; ++i) {
		Fifo f(NamingService::getDealingFifoName(i));
		this->dealingFifos.push_back(f);
		Semaphore s(NamingService::getSemaphoreKey(SemaphoreNames::DealtSemaphore, i));
		this->dealtSemaphores.push_back(s);
	}
}

void Table::run(){
	string message = "Table starting";
	Logger::getInstance()-> logLine(message, INFO);
	int loserId = -1;
	do {
		this->deal();
		this->unblockPlayers();

		do{
			loserId = this->checkForLoser();
		} while(loserId == -1);

	} while(!this->scoreboard.trackLost(loserId));

	message = "Table - GAME OVER";
	Logger::getInstance()-> logLine(message, INFO);

	this->notifyGameOver();
}

int Table::checkForLoser(void){
	string message;
	int decidedOnHandDown = 0;
	int playerId = -1;
	int handsDown = 0;
	while(decidedOnHandDown != this->numberOfPlayers){
		message = "Waiting for player to decide if won";
		Logger::getInstance()-> logLine(message, INFO);

		char id[4];
		memset(id, 0, 4);
		this->handDownFifo.readValue(id, sizeof(char) * 4);
		playerId = id[0] + (id[1] << 8) + (id[2] << 16) + (id[3] << 24);

		if (playerId != -1){
			handsDown++;
			message = "Player put hand down: " + Convert::toString(playerId);
			Logger::getInstance()-> logLine(message, INFO);
		}

		decidedOnHandDown++;
	}

	for (int i = 0; i < this->numberOfPlayers; i++){
		char valueToWrite[1];
		valueToWrite[0] = 0;
		if (handsDown != 0){
			valueToWrite[0] = 1;
		}

		this->playerWonFifo.writeValue(valueToWrite, sizeof(char));
	}

	if (handsDown != 0){
		while (handsDown != this->numberOfPlayers){
			char id[4];
			memset(id, 0, 4);
			this->handDownFifo.readValue(id, sizeof(char) * 4);
			playerId = id[0] + (id[1] << 8) + (id[2] << 16) + (id[3] << 24);
			handsDown++;
			message = "Player put hand down: " + Convert::toString(playerId);
			Logger::getInstance()-> logLine(message, INFO);
		}

		message = "Table - Round Finished.  Player that lost: " + Convert::toString(playerId);
		Logger::getInstance()-> logLine(message, INFO);

		return playerId;
	}

	return -1;
}

void Table::deal(){
	PigGoesDeck deck(this->numberOfPlayers);
	deck.shuffle();

	int i = 0;

	while (deck.hasNextCard()){
		Card card = deck.hit();
		char c[2];
		memset(c, 0, 2);
		card.serialize(c);
		string m = "Dealing card " + Convert::toString(i);
		Logger::getInstance()-> logLine(m, INFO);
		int result = this->dealingFifos[i % this->numberOfPlayers].writeValue(c, sizeof(char) * 2);
		m = "Dealing FIFO result " + Convert::toString(result);
		Logger::getInstance()-> logLine(m, INFO);
		i++;
	}

	Logger::getInstance()-> logLine("Finished dealing", INFO);
}

void Table::unblockPlayers(void){
	for (unsigned int i = 0; i < this->dealtSemaphores.size(); i++) {
		string s = "Signaling semaphore " + Convert::toString(i);
		Logger::getInstance()-> logLine(s, INFO);
		this->dealtSemaphores[i].signal();
	}
}

void Table::notifyGameOver(){
	string message;
	for (unsigned int i = 0; i < this->playerProcesses.size(); ++i) {
		message = "Notifying game over to player - " + Convert::toString(i);
		Logger::getInstance()-> logLine(message, INFO);
		kill(this->playerProcesses[i], SignalNumbers::GameOver);
	}

	message = "Notifying game over to player Sync";
	Logger::getInstance()-> logLine(message, INFO);
	kill(this->syncProcessId, SignalNumbers::GameOver);
}

Table::~Table() {
	for (unsigned int i = 0; i < this->dealingFifos.size(); i++) {
		this->dealingFifos[i].closeFifo();
	}

	string message = "Destroying table";
	Logger::getInstance()-> logLine(message, INFO);
}
