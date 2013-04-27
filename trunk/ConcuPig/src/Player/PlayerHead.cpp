#include "PlayerHead.h"
#include "../Services/NamingService.h"
#include "../Constants/SemaphoreNames.h"
#include "../Constants/SharedMemoryNames.h"
#include "../Constants/SignalNumbers.h"
#include <map>

PlayerHead::PlayerHead(pid_t fatherId, int playerNumber, int leftPlayerNumber, int rightPlayerNumber) :
	fatherId(fatherId),
	number(playerNumber),
	leftPlayerNumber(leftPlayerNumber),
	rightPlayerNumber(rightPlayerNumber),
	readyToSendReceiveSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::ReadyToSendReceive, playerNumber)),
	receiverSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::ReceiverSemaphore, playerNumber)),
	senderSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::SenderSemaphore, playerNumber)),
	receivedSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::ReceivedSemaphore, playerNumber)),
	sentSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::SentSemaphore, playerNumber)),
	cardToSendMemory(SharedMemoryNames::CardToSend, playerNumber),
	receivedCardMemory(SharedMemoryNames::CardToReceive, playerNumber)
{ }

void PlayerHead::playRound()
{
	// state : preparing to play a round
	Card cardToSend = retrieveCardToSend();
	informCardHasBeenSelected();
	readyToSendReceiveSemaphore.wait();

	// state : playing round
	cardToSendMemory.setCard(cardToSend);
	senderSemaphore.signal();
	receiverSemaphore.signal();
	sentSemaphore.wait();
	receivedSemaphore.wait();
	Card receivedCard = receivedCardMemory.getCard();
	this->hand.push_back(receivedCard);

	// state: checking if i win
	if( isWinningHand() )
	{
		informMyHandIsOnTheTable();
	}
}

void PlayerHead::takeCard( Card card )
{
	this->hand.push_back(card);
}

Card PlayerHead::retrieveCardToSend()
{
	// TODO implement a stategy to decide the position o the card to grab
	std::map<char, int> dictionary;
	for(unsigned int indexCard = 0 ; indexCard < this->hand.size() ; indexCard++)
	{
		Card card = this->hand[indexCard];
		std::map<char, int>::iterator it = dictionary.find(card.getSymbol());
		int ocurrencies = 1;
		if( dictionary.end() != it )
		{
			ocurrencies += it->second;
		}
		dictionary[card.getSymbol()] = ocurrencies;
	}
	std::map<char, int>::iterator minItMap = dictionary.begin();
	std::map<char, int>::iterator itMap = dictionary.begin();
	while( dictionary.end() != itMap )
	{
		if( minItMap->second > itMap->second )
		{
			minItMap = itMap;
		}
		itMap++;
	}
	bool notFountSymbol = true;
	int position = 0;
	while( notFountSymbol )
	{
		if( this->hand[position].getSymbol() == minItMap->first )
			notFountSymbol = false;
		else
			position++;
	}

	Card returnedCard = this->hand[position];
	std::vector<Card>::iterator it = this->hand.begin();
	advance(it, position);
	this->hand.erase(it);
	return returnedCard;
}

void PlayerHead::informCardHasBeenSelected()
{
	kill(this->fatherId , SignalNumber::CardSelected);
}

void PlayerHead::informMyHandIsOnTheTable()
{
	kill(this->fatherId , SignalNumber::PlayerWon);
}

bool PlayerHead::isWinningHand()
{
	return
			(this->hand[0].getSymbol() == this->hand[1].getSymbol()) &&
			(this->hand[1].getSymbol() == this->hand[2].getSymbol()) &&
			(this->hand[2].getSymbol() == this->hand[3].getSymbol());
}

PlayerHead::~PlayerHead() { }

