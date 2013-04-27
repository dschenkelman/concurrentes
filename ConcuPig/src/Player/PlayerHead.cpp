/*
 * PlayerHead.cpp
 *
 *  Created on: 27/04/2013
 *      Author: matias
 */

#include "PlayerHead.h"
#include "../Services/NamingService.h"
#include "../Constants/SemaphoreNames.h"

PlayerHead::PlayerHead(int position, int leftPlayerPosition, int rightPlayerPosition) :
	position(position),
	leftPlayerPosition(leftPlayerPosition),
	rightPlayerPosition(rightPlayerPosition),
	readyToSendReceiveSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::ReadyToSendReceive, position))
{
	NamingService::getSemaphoreKey(SemaphoreNames::ReadyToSendReceive, position);

}

void PlayerHead::playRound()
{
	Card cardToSend = retrieveCardToSend();
	informCardHasBeenSelected();
	readyToSendReceiveSemaphore.wait();


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

