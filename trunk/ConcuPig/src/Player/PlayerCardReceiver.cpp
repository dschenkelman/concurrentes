#include "PlayerCardReceiver.h"
#include "../Constants/SemaphoreNames.h"
#include "../Constants/FifoNames.h"
#include "../Constants/SharedMemoryNames.h"

PlayerCardReceiver::PlayerCardReceiver(int playerNumber, int playerOrigin):
	receiverSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::ReceiverSemaphore,playerNumber)),
	receivedSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::ReceivedSemaphore,playerNumber)),
	fifo(NamingService::getCardPassingFifoFileName(playerOrigin,playerNumber)),
	sharedCard(SharedMemoryNames::CardToReceive,playerNumber){

}

PlayerCardReceiver::~PlayerCardReceiver(){

}

void PlayerCardReceiver::run(){
	while(true){

		receiverSemaphore.wait();

		char cardReceived [2];
		this->fifo.readValue(cardReceived,2);

		this->sharedCard.deSerialize(cardReceived);

		receivedSemaphore.signal();
	}
}
