#include "SharedCard.h"

using namespace std;

SharedCard::SharedCard (string& cardType, int playerNumber){

	key_t cardKey = NamingService::getSharedCardKey(cardType,playerNumber);

	this->sharedCardMemory.create(cardKey);

	this->sharedMemoryState = 2;
}

SharedCard::~SharedCard(){

}
