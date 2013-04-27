#ifndef SHAREDCARD_H_
#define SHAREDCARD_H_
#include "../Concurrency/SharedMemory.h"
#include "../Concurrency/Lock.h"
#include "../Services/NamingService.h"
#include "Card.h"

class SharedCard {

private:
	SharedMemory<Card> sharedCardMemory;
	int sharedMemoryState;
	std::string fileName;

public:

	SharedCard (const std::string& cardType, int playerNumber);
	virtual ~SharedCard();

	void setCard(Card card);
	Card getCard();

	void serialize(char* buffer);

	void deSerialize(char* serializedCard);

};

#endif /* SHAREDCARD_H_ */
