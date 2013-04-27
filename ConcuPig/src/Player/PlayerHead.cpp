/*
 * PlayerHead.cpp
 *
 *  Created on: 27/04/2013
 *      Author: matias
 */

#include "PlayerHead.h"
#include "../Services/NamingService.h"
#include "../Constants/SemaphoreNames.h"
#include "../Constants/SharedMemoryNames.h"

PlayerHead::PlayerHead(int playerNumber, int leftPlayerNumber, int rightPlayerNumber) :
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

	// state: checking if i win

}

Card PlayerHead::retrieveCardToSend()
{
	// TODO implment a stategy to decide the position o the card to grab
	int position = 0;
	//

	Card returnedCard = this->hand[position];
	vector<Card>::iterator it = this->hand.begin();
	advance(it, position);
	this->hand.erase(it);
	return returnedCard;
}

void PlayerHead::informCardHasBeenSelected()
{
	// TODO implement
}

PlayerHead::~PlayerHead() { }

