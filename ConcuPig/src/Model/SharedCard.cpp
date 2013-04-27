#include "SharedCard.h"

using namespace std;

SharedCard::SharedCard (string& cardType, int playerNumber){

	key_t cardKey = NamingService::getSharedCardKey(cardType,playerNumber);

	this->sharedMemoryState = this->sharedCardMemory.create(cardKey);

	this->fileName = NamingService::getSharedCardFileName(cardType,playerNumber);

}

SharedCard::~SharedCard(){
	this->sharedCardMemory.release();
}


void SharedCard::setCard(Card card){
	{
		Lock l(this->fileName);

		this->sharedCardMemory.setValue(card);
	}

}

Card SharedCard::getCard(){
	{
		Lock l(this->fileName);

		return (this->sharedCardMemory.getValue());
	}
}
