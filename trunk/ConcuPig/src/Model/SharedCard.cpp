#include "SharedCard.h"
#include "../Services/Logger.h"
#include <string>

using namespace std;

SharedCard::SharedCard (const string& cardType, int playerNumber){

	key_t cardKey = NamingService::getSharedCardKey(cardType,playerNumber);

	this->fileName = NamingService::getSharedCardFileName(cardType,playerNumber);

	Lock l(this->fileName);

	this->sharedMemoryState = this->sharedCardMemory.create(cardKey);
}

SharedCard::~SharedCard(){
	Lock l(this->fileName);

	this->sharedCardMemory.release();

	string message = "Releasing card";

	Logger::getInstance()->logLine(message, INFO);
}


void SharedCard::setCard(Card card){
	Lock l(this->fileName);

	this->sharedCardMemory.setValue(card);
}

Card SharedCard::getCard(){
	Lock l(this->fileName);

	return (this->sharedCardMemory.getValue());
}

void SharedCard::serialize(char* buffer){
	this->getCard().serialize(buffer);
}

void SharedCard::deSerialize(char* serializedCard){
	Card c(serializedCard[0],serializedCard[1]);
	setCard(c);
}
