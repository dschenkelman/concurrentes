#include "PlayerSynchronizer.h"
#include "../Services/NamingService.h"
#include "../Constants/SemaphoreNames.h"

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

	int playerId = -1;

	int playersReady = 0;
	while(playersReady != this->numberOfPlayers){

		this->playersReadyFifo.readValue((char*)&playerId, sizeof(int));
		playersReady++;
	}

	this->unblockPlayers();
}

void PlayerSynchronizer::unblockPlayers(void){
	for (unsigned int i = 0; i < this->playersReadySemaphores.size(); i++) {
		this->playersReadySemaphores[i].signal();
	}
}

