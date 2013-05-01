#include "PlayerSynchronizer.h"
#include "../Services/NamingService.h"
#include "../Constants/SemaphoreNames.h"
#include "../Services/Logger.h"
#include <string>

using namespace std;

PlayerSynchronizer::PlayerSynchronizer(int numberOfPlayers):
	playersReadyFifo(NamingService::getPlayersReadyFifoName()), numberOfPlayers(numberOfPlayers){
	for (int i = 0; i < numberOfPlayers; ++i) {
		Semaphore s(NamingService::getSemaphoreKey(SemaphoreNames::ReadyToSendReceive, i));
		this->playersReadySemaphores.push_back(s);
	}
}

PlayerSynchronizer::~PlayerSynchronizer(){

}

void PlayerSynchronizer::run(){
	while (true){
		int playerId = -1;

		string line = "Sync - Starting";
		Logger::getInstance()->logLine(line, INFO);

		int playersReady = 0;
		while(playersReady != this->numberOfPlayers){
			string line = "Sync - Reading players ready FIFO";
			Logger::getInstance()->logLine(line, INFO);

			char id[4];

			this->playersReadyFifo.readValue(id, sizeof(char) * 4);

			playerId = id[0] + (id[1] << 8) + (id[2] << 16) + (id[3] << 24);

			line = "Sync - Player Id ready in Sync: " + Convert::ToString(playerId);
			Logger::getInstance()->logLine(line, INFO);
			playersReady++;
			line = "Sync - Players Ready: " + Convert::ToString(playersReady);
			Logger::getInstance()->logLine(line, INFO);
		}

		this->unblockPlayers();
	}
}

void PlayerSynchronizer::unblockPlayers(void){
	for (unsigned int i = 0; i < this->playersReadySemaphores.size(); i++) {
		this->playersReadySemaphores[i].signal();
		string line = "Sync - Unblocked player " + Convert::ToString(i);
		Logger::getInstance()->logLine(line, INFO);
	}
}
