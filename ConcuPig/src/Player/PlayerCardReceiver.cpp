#include "PlayerCardReceiver.h"
#include "../Constants/SemaphoreNames.h"
#include "../Constants/FifoNames.h"
#include "../Constants/SharedMemoryNames.h"
#include "../Services/Logger.h"

using namespace std;

PlayerCardReceiver::PlayerCardReceiver(int playerNumber, int playerOrigin):
	receiverSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::ReceiverSemaphore,playerNumber)),
	receivedSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::ReceivedSemaphore,playerNumber)),
	fifo(NamingService::getCardPassingFifoFileName(playerOrigin,playerNumber)),
	sharedCard(SharedMemoryNames::CardToReceive,playerNumber){

	this->playerNumber = playerNumber;
	this->playerOrigin = playerOrigin;
	this->fifoName = NamingService::getCardPassingFifoFileName(playerOrigin,playerNumber);
}

PlayerCardReceiver::~PlayerCardReceiver(){

}

void PlayerCardReceiver::run(){
	while(true){
		Logger *logger = Logger::getInstance();

		string logLine = "Waiting on Receiver Semaphore";
		logger->logPlayer(this->playerNumber,logLine,INFO);
		receiverSemaphore.wait();

		char cardReceived [2];
		logLine = "Reading from FIFO "+this->fifoName+". Sent by Player: "+Convert::ToString(this->playerOrigin)+" to: Player: "+
						Convert::ToString(this->playerNumber);
		logger->logLine(logLine,INFO);
		this->fifo.readValue(cardReceived,2);

		logLine = "Deserializing and writing new card on shared memory";
		logger->logPlayer(this->playerNumber,logLine,INFO);
		this->sharedCard.deSerialize(cardReceived);

		logLine = "Signaling on Received Semaphore";
		logger->logPlayer(this->playerNumber,logLine,INFO);
		receivedSemaphore.signal();
	}
}
