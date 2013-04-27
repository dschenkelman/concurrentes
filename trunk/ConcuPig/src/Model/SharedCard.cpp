#include "SharedCard.h"

using namespace std;

SharedCard::SharedCard (const string& cardType, int playerNumber){

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

void SharedCard::serialize(char* buffer){
	Card sharedCard = this->getCard();
	buffer[0] = sharedCard.getNumber();
	buffer[1] = sharedCard.getSymbol();
}

void SharedCard::deSerialize(char* serializedCard){
	Card c(serializedCard[0],serializedCard[1]);
	setCard(c);
}
