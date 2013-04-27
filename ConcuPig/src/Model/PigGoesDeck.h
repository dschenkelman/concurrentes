#ifndef PIGGOESDECK_H_
#define PIGGOESDECK_H_

#include <list>
#include "Card.h"

using namespace std;

class PigGoesDeck {

private:

	int participantsCount;
	int cardsPerSymbolCount;
	std::list<Card> cards;

public:

	PigGoesDeck( int participantsCount );
	virtual ~PigGoesDeck();

	void shuffle();
	bool hasNextCard();
	Card hit();
};

#endif /* PIGGOESDECK_H_ */
