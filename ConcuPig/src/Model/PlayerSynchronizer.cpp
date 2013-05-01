#include "PlayerSynchronizer.h"
#include "../Services/NamingService.h"
#include "../Constants/SemaphoreNames.h"
#include "../Services/Logger.h"
#include <string>
#include "../Concurrency/SignalHandler.h"
#include "../Constants/SignalNumbers.h"

#define SIG_ATOMIC_FALSE 0
#define SIG_ATOMIC_TRUE 1

using namespace std;

PlayerSynchronizer::PlayerSynchronizer(int numberOfPlayers):
	gameOver(SIG_ATOMIC_FALSE),
	playersReadyFifo(NamingService::getPlayersReadyFifoName()), numberOfPlayers(numberOfPlayers){
	for (int i = 0; i < numberOfPlayers; ++i) {
		Semaphore s(NamingService::getSemaphoreKey(SemaphoreNames::ReadyToSendReceive, i));
		this->playersReadySemaphores.push_back(s);
	}
}

PlayerSynchronizer::~PlayerSynchronizer(){

}

void PlayerSynchronizer::run(){
	while (this->gameOver == SIG_ATOMIC_FALSE){
		int playerId = -1;

		string line = "Sync - Starting";
		Logger::getInstance()->logLine(line, INFO);

		int playersReady = 0;
		while(playersReady != this->numberOfPlayers && (this->gameOver == SIG_ATOMIC_FALSE)){
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

int PlayerSynchronizer::handleSignal (int signum){
	if (signum == SignalNumbers::GameOver){
		this->gameOver = SIG_ATOMIC_TRUE;
	}
	else {
		return -1;
	}

	return 0;
}
