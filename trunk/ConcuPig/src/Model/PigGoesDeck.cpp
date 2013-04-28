#include "PigGoesDeck.h"
#include <cstdlib>
#include <time.h>

PigGoesDeck::PigGoesDeck( int participantsCount ) :
	participantsCount(participantsCount),
	cardsPerSymbolCount(4)
{
	shuffle();
}

void PigGoesDeck::shuffle()
{
	this->cards.clear();
	for( int participantsIndex = 0 ; participantsIndex < this->cardsPerSymbolCount ; participantsIndex++ )
	{
		for( int cardNumber = 0 ; cardNumber < this->participantsCount ; cardNumber++ )
		{
			int insertIndex = rand() % (this->cards.size() + 1);
			char symbol = participantsIndex;
			char number = cardNumber;
			std::list<Card>::iterator it = this->cards.begin();
			advance(it, insertIndex);
			this->cards.insert(it, Card(number, symbol));
		}
	}
}

bool PigGoesDeck::hasNextCard()
{
	return this->cards.size() > 0;
}

Card PigGoesDeck::hit()
{
	Card retrievedCart = *this->cards.begin();
	this->cards.pop_front();
	return retrievedCart;
}

PigGoesDeck::~PigGoesDeck() { }

