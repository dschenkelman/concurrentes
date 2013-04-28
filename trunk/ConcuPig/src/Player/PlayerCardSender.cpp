#include "PlayerCardSender.h"
#include "../Constants/SemaphoreNames.h"
#include "../Constants/FifoNames.h"
#include "../Constants/SharedMemoryNames.h"

using namespace std;

PlayerCardSender::PlayerCardSender(int playerNumber, int playerTarget):
	senderSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::SenderSemaphore,playerNumber)),
	sentSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::SentSemaphore,playerNumber)),
	fifo(NamingService::getCardPassingFifoFileName(playerNumber,playerTarget)),
	sharedCard(SharedMemoryNames::CardToSend,playerNumber){

	this->fifoName = NamingService::getCardPassingFifoFileName(playerNumber,playerTarget);
	this->playerNumber = playerNumber;
	this->playerTarget = playerTarget;

}

PlayerCardSender::~PlayerCardSender(){

}

void PlayerCardSender::run(){
	while(true){
		Logger *logger = Logger::getInstance();

		string logLine = "Waiting on Sender Semaphore";
		logger->logPlayer(this->playerNumber,logLine,INFO);
		senderSemaphore.wait();

		char cardToSend [2];
		this->sharedCard.serialize(cardToSend);

		logLine = "Writing on FIFO "+this->fifoName+" from Player: "+Convert::ToString(this->playerNumber)+" to: Player: "+
				Convert::ToString(this->playerTarget);
		logger->logLine(logLine,INFO);
		this->fifo.writeValue(cardToSend, sizeof(char) * 2);

		logLine = "Signaling on Sent Semaphore";
		logger->logPlayer(this->playerNumber,logLine,INFO);
		sentSemaphore.signal();
	}
}
