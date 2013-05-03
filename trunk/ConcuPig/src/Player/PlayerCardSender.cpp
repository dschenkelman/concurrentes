#include "PlayerCardSender.h"
#include "../Constants/SemaphoreNames.h"
#include "../Constants/FifoNames.h"
#include "../Constants/SharedMemoryNames.h"
#include "../Concurrency/SignalHandler.h"
#include "../Constants/SignalNumbers.h"
#include <cstring>

#define SIG_ATOMIC_FALSE 0
#define SIG_ATOMIC_TRUE 1

using namespace std;

PlayerCardSender::PlayerCardSender(int playerNumber, int playerTarget):
	gameOver(SIG_ATOMIC_FALSE),
	senderSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::SenderSemaphore,playerNumber)),
	sentSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::SentSemaphore,playerNumber)),
	fifo(NamingService::getCardPassingFifoFileName(playerNumber,playerTarget)),
	sharedCard(SharedMemoryNames::CardToSend,playerNumber){

	SignalHandler::getInstance()->registerHandler(SignalNumbers::GameOver, this);
	this->fifoName = NamingService::getCardPassingFifoFileName(playerNumber,playerTarget);
	this->playerNumber = playerNumber;
	this->playerTarget = playerTarget;

}

PlayerCardSender::~PlayerCardSender(){
	string message = "Sender - Destroying";
	Logger::getInstance()->logPlayer(this->playerNumber, message, INFO);
}

void PlayerCardSender::run(){
	Logger *logger = Logger::getInstance();

	string logLine;
	while(this->gameOver == SIG_ATOMIC_FALSE){
		logLine = "Waiting on Sender Semaphore";
		logger->logPlayer(this->playerNumber,logLine,INFO);
		senderSemaphore.wait();

		char cardToSend [2];
		memset(cardToSend, 0, 2);
		this->sharedCard.serialize(cardToSend);

		logLine = "Writing on FIFO "+this->fifoName+" from Player: "+Convert::toString(this->playerNumber)+" to: Player: "+
				Convert::toString(this->playerTarget);
		logger->logPlayer(this->playerNumber, logLine,INFO);
		this->fifo.writeValue(cardToSend, sizeof(char) * 2);

		logLine = "Signaling on Sent Semaphore";
		logger->logPlayer(this->playerNumber,logLine,INFO);
		sentSemaphore.signal();
	}

	logLine = "Sender - Ending";
	logger->logPlayer(this->playerNumber,logLine,INFO);
}

int PlayerCardSender::handleSignal (int signum){
	if (signum == SignalNumbers::GameOver){
		this->gameOver = SIG_ATOMIC_TRUE;
		return 0;
	}

	return -1;
}
