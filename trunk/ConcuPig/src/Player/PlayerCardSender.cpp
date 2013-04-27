#include "PlayerCardSender.h"
#include "../Constants/SemaphoreNames.h"
#include "../Constants/FifoNames.h"
#include "../Constants/SharedMemoryNames.h"

PlayerCardSender::PlayerCardSender(int playerNumber, int playerTarget):
	senderSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::SenderSemaphore,playerNumber)),
	sentSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::SentSemaphore,playerNumber)),
	fifo(NamingService::getCardPassingFifoFileName(playerNumber,playerTarget)),
	sharedCard(SharedMemoryNames::CardToSend,playerNumber){
}

PlayerCardSender::~PlayerCardSender(){

}

void PlayerCardSender::run(){
	while(true){

		senderSemaphore.wait();

		char cardToSend [2];
		this->sharedCard.serialize(cardToSend);

		this->fifo.writeValue(cardToSend,2);

		sentSemaphore.signal();
	}
}
