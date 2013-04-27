#ifndef SHAREDCARD_H_
#define SHAREDCARD_H_
#include "../Concurrency/SharedMemory.h"
#include "../Services/NamingService.h"

class SharedCard {

private:


public:

	SharedCard (std::string& cardType, int playerNumber);
	virtual ~SharedCard();

};

#endif /* SHAREDCARD_H_ */
