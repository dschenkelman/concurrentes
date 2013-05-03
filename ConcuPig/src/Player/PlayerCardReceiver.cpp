#include "PlayerCardReceiver.h"
#include "../Constants/SemaphoreNames.h"
#include "../Constants/FifoNames.h"
#include "../Constants/SharedMemoryNames.h"
#include "../Services/Logger.h"
#include "../Concurrency/SignalHandler.h"
#include "../Constants/SignalNumbers.h"
#include <cstring>

#define SIG_ATOMIC_FALSE 0
#define SIG_ATOMIC_TRUE 1

using namespace std;

PlayerCardReceiver::PlayerCardReceiver(int playerNumber, int playerOrigin):
	gameOver(SIG_ATOMIC_FALSE),
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
	string message = "Receiver - Destroying";
	Logger::getInstance()->logPlayer(this->playerNumber, message, INFO);
}

void PlayerCardReceiver::run(){
	string logLine;
	Logger *logger = Logger::getInstance();
	while(this->gameOver == SIG_ATOMIC_FALSE){
		logLine = "Waiting on Receiver Semaphore";
		logger->logPlayer(this->playerNumber,logLine,INFO);
		receiverSemaphore.wait();

		char cardReceived [2];
		memset(cardReceived, 0, 2);
		logLine = "Reading from FIFO "+this->fifoName+". Sent by Player: "+Convert::toString(this->playerOrigin)+" to: Player: "+
						Convert::toString(this->playerNumber);
		logger->logPlayer(this->playerNumber, logLine, INFO);
		this->fifo.readValue(cardReceived, sizeof(char) * 2);

		logLine = "Deserializing and writing new card on shared memory";
		logger->logPlayer(this->playerNumber,logLine,INFO);
		this->sharedCard.deSerialize(cardReceived);

		logLine = "Signaling on Received Semaphore";
		logger->logPlayer(this->playerNumber,logLine,INFO);
		receivedSemaphore.signal();
	}

	logLine = "Receiver - Ending";
	logger->logPlayer(this->playerNumber,logLine,INFO);
}

int PlayerCardReceiver::handleSignal (int signum){
	if (signum == SignalNumbers::GameOver){
		this->gameOver = SIG_ATOMIC_TRUE;
	}
	else {
		return -1;
	}

	return 0;
}
