#include "PlayerCardReceiver.h"
#include "../Constants/SemaphoreNames.h"
#include "../Constants/FifoNames.h"
#include "../Constants/SharedMemoryNames.h"
#include "../Services/Logger.h"
#include "../Concurrency/SignalHandler.h"
#include "../Constants/SignalNumbers.h"

using namespace std;

PlayerCardReceiver::PlayerCardReceiver(int playerNumber, int playerOrigin):
	gameOver(false),
	receiverSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::ReceiverSemaphore,playerNumber)),
	receivedSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::ReceivedSemaphore,playerNumber)),
	fifo(NamingService::getCardPassingFifoFileName(playerOrigin,playerNumber)),
	sharedCard(SharedMemoryNames::CardToReceive,playerNumber){

	SignalHandler::getInstance()->registerHandler(SignalNumbers::GameOver, this);
	this->playerNumber = playerNumber;
	this->playerOrigin = playerOrigin;
	this->fifoName = NamingService::getCardPassingFifoFileName(playerOrigin,playerNumber);
}

PlayerCardReceiver::~PlayerCardReceiver(){

}

void PlayerCardReceiver::run(){
	while(!this->gameOver){
		Logger *logger = Logger::getInstance();

		string logLine = "Waiting on Receiver Semaphore";
		logger->logPlayer(this->playerNumber,logLine,INFO);
		receiverSemaphore.wait();

		char cardReceived [2];
		logLine = "Reading from FIFO "+this->fifoName+". Sent by Player: "+Convert::ToString(this->playerOrigin)+" to: Player: "+
						Convert::ToString(this->playerNumber);
		logger->logLine(logLine,INFO);
		this->fifo.readValue(cardReceived, sizeof(char) * 2);

		logLine = "Deserializing and writing new card on shared memory";
		logger->logPlayer(this->playerNumber,logLine,INFO);
		this->sharedCard.deSerialize(cardReceived);

		logLine = "Signaling on Received Semaphore";
		logger->logPlayer(this->playerNumber,logLine,INFO);
		receivedSemaphore.signal();
	}
}

int PlayerCardReceiver::handleSignal (int signum){
	if (signum == SignalNumbers::GameOver){
		this->gameOver = true;
	}
	else {
		return -1;
	}

	return 0;
}
