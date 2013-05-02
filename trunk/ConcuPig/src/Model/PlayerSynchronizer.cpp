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
	playersReadyFifo(NamingService::getPlayersReadyFifoName()),
	playersEverybodyPickUpCardFifo(NamingService::getPlayersEverybodyPickUpCardFifoName()),
	numberOfPlayers(numberOfPlayers){
	for (int i = 0; i < numberOfPlayers; ++i) {
		Semaphore s(NamingService::getSemaphoreKey(SemaphoreNames::ReadyToSendReceive, i));
		this->playersReadySemaphores.push_back(s);

		Semaphore a(NamingService::getSemaphoreKey(SemaphoreNames::EverybodyPickUpCardSemaphore, i));
		this->playersEverybodyPickUpCardSemaphores.push_back(a);
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

		this->unblockPlayersReady();

		line = "Sync - Finishing round synchronization";
		Logger::getInstance()->logLine(line, INFO);

		int playersFinished = 0;
		while(playersFinished != this->numberOfPlayers && (this->gameOver == SIG_ATOMIC_FALSE)){
			string line = "Sync - Reading players finished FIFO";
			Logger::getInstance()->logLine(line, INFO);

			char id[4];
			this->playersEverybodyPickUpCardFifo.readValue(id, sizeof(char) * 4);
			playerId = id[0] + (id[1] << 8) + (id[2] << 16) + (id[3] << 24);

			line = "Sync - Player Id finished in Sync: " + Convert::ToString(playerId);
			Logger::getInstance()->logLine(line, INFO);
			playersFinished++;

			line = "Sync - Players finished: " + Convert::ToString(playersFinished);
			Logger::getInstance()->logLine(line, INFO);
		}

		this->unblockPlayersFinished();
	}
}

void PlayerSynchronizer::unblockPlayersReady(void){
	for (unsigned int i = 0; i < this->playersReadySemaphores.size(); i++) {
		this->playersReadySemaphores[i].signal();
		string line = "Sync - Unblocked player ready " + Convert::ToString(i);
		Logger::getInstance()->logLine(line, INFO);
	}
}

void PlayerSynchronizer::unblockPlayersFinished(void){
	for (unsigned int i = 0; i < this->playersEverybodyPickUpCardSemaphores.size(); i++) {
		this->playersEverybodyPickUpCardSemaphores[i].signal();
		string line = "Sync - Unblocked player finished " + Convert::ToString(i);
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
