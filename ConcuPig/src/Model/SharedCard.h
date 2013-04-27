#ifndef SHAREDCARD_H_
#define SHAREDCARD_H_
#include "../Concurrency/SharedMemory.h"
#include "../Services/NamingService.h"
#include "Card.h"

class SharedCard {

private:
	SharedMemory<Card> sharedCardMemory;
	int sharedMemoryState;

public:

	SharedCard (std::string& cardType, int playerNumber);
	virtual ~SharedCard();

	void setCard(Card card);

	Card getCard();

};

#endif /* SHAREDCARD_H_ */
