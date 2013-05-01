#include "PlayerHead.h"
#include "PlayerCardReceiver.h"
#include "PlayerCardSender.h"
#include "../Services/NamingService.h"
#include "../Constants/SemaphoreNames.h"
#include "../Constants/SharedMemoryNames.h"
#include "../Constants/SignalNumbers.h"
#include "../Services/Logger.h"
#include "../Services/LoggerLevels.h"
#include "../Helpers/Convert.h"
#include <map>
#include <string>
#include "../Concurrency/Lock.h"
#include "../Concurrency/SignalHandler.h"
#include <cstdlib>
#include <algorithm>

using namespace std;


PlayerHead::PlayerHead( int playerNumber, pid_t receiverProcess, pid_t senderProcess) :
playingRound(false),
gameOver(false),
receiverProcessId(receiverProcess),
senderProcessId(senderProcess),
number(playerNumber),
	readyToSendReceiveSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::ReadyToSendReceive, playerNumber)),
	receiverSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::ReceiverSemaphore, playerNumber)),
	senderSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::SenderSemaphore, playerNumber)),
	receivedSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::ReceivedSemaphore, playerNumber)),
	sentSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::SentSemaphore, playerNumber)),
	dealtSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::DealtSemaphore, playerNumber)),
	cardToSendMemory(SharedMemoryNames::CardToSend, playerNumber),
	receivedCardMemory(SharedMemoryNames::CardToReceive, playerNumber),
	playerReadyFifo(NamingService::getPlayersReadyFifoName()),
	handDownFifo(NamingService::getHandDownFifoName()),
	dealtFifo(NamingService::getDealingFifoName(playerNumber))
{
	SignalHandler::getInstance()->registerHandler(SignalNumbers::PlayerWon, this);
	SignalHandler::getInstance()->registerHandler(SignalNumbers::GameOver, this);
}

void PlayerHead::takeCard( Card card )
{
	this->hand.push_back(card);
}

Card PlayerHead::retrieveCardToSend()
{
	std::map<char, int> dictionary;
	for(unsigned int indexCard = 0 ; indexCard < this->hand.size() ; indexCard++)
	{
		Card card = this->hand[indexCard];
		std::map<char, int>::iterator it = dictionary.find(card.getNumber());
		int ocurrencies = 1;
		if( dictionary.end() != it )
		{
			ocurrencies += it->second;
		}
		dictionary[card.getNumber()] = ocurrencies;
	}
	int minOcurrence = 1000;
	std::map<char, int>::iterator itMap = dictionary.begin();
	while( dictionary.end() != itMap )
	{
		if( minOcurrence > itMap->second )
		{
			minOcurrence = itMap->second;
		}
		itMap++;
	}
	std::vector<char> numbersWithMinOccurrencies;
	itMap = dictionary.begin();
	while( dictionary.end() != itMap )
	{
		if( minOcurrence == itMap->second )
		{
			numbersWithMinOccurrencies.push_back(itMap->first);
		}
		itMap++;
	}
	std::sort(numbersWithMinOccurrencies.begin(), numbersWithMinOccurrencies.end());
	char numberSelected = numbersWithMinOccurrencies[(rand() % (numbersWithMinOccurrencies.size() + numbersWithMinOccurrencies.size() / 2 )) % numbersWithMinOccurrencies.size() ];
	std::vector<int> positionsOfCandidateCards;
	for( unsigned int i = 0 ; i < this->hand.size() ; i++ )
	{
		if( this->hand[i].getNumber() == numberSelected )
			positionsOfCandidateCards.push_back(i);
	}
	int positionSelected = positionsOfCandidateCards[rand() % positionsOfCandidateCards.size()];
	Card returnedCard = this->hand[positionSelected];
	std::vector<Card>::iterator it = this->hand.begin();
	advance(it, positionSelected);
	this->hand.erase(it);
	return returnedCard;
}

void PlayerHead::informCardHasBeenSelected()
{
	char n[4];
	n[0] = this->number & 255;
	n[1] = (this->number >> 8 ) & 255;
	n[2] = (this->number >> 16) & 255;
	n[3] = (this->number >> 24 ) & 255;

	std::string message = "playerReadyFifo.writeValue";
	Logger::getInstance()->logPlayer(this->number, message, INFO);

	this->playerReadyFifo.writeValue(n, sizeof(char) * 4);
}

void PlayerHead::informMyHandIsOnTheTable()
{
	char n[4];
	n[0] = this->number & 255;
	n[1] = (this->number >> 8 ) & 255;
	n[2] = (this->number >> 16) & 255;
	n[3] = (this->number >> 24 ) & 255;

	std::string message = "handDownFifo.writeValue";
	Logger::getInstance()->logPlayer(this->number, message, INFO);

	this->handDownFifo.writeValue(n, sizeof(char) * 4);
}

bool PlayerHead::isWinningHand()
{
	return
			(this->hand[0].getNumber() == this->hand[1].getNumber()) &&
			(this->hand[1].getNumber() == this->hand[2].getNumber()) &&
			(this->hand[2].getNumber() == this->hand[3].getNumber());
}

void PlayerHead::run()
{
	std::string message;

	while(!this->gameOver)
	{
		this->hand.clear();

//		if (this->gameOver){
//			message = "Exiting game";
//			Logger::getInstance()->logPlayer(this->number, message, INFO);
//			break;
//		}

		for( int i = 0 ; i < 4 ; i++)
		{
			char serializedCard[2];
			message = "dealtFifo.readValue";
			Logger::getInstance()->logPlayer(this->number, message, INFO);
			dealtFifo.readValue(serializedCard, 2);

			Card card(serializedCard[0], serializedCard[1]);
			this->hand.push_back(card);

			Logger::getInstance()->logPlayer(this->number, card.toString(), INFO);
		}

		this->logHand();

		message = "dealtSemaphore.wait";
		Logger::getInstance()->logPlayer(this->number, message, INFO);
		dealtSemaphore.wait();

		this->playingRound = true;
/*
		if( isWinningHand() )
		{
			informMyHandIsOnTheTable();
			playingRound = false;
		}
		*/

		int playingRoundCount = 0;
		while( playingRound )
		{
			message = "Playing round = " + Convert::ToString(playingRoundCount);
			Logger::getInstance()->logPlayer(this->number, message, INFO);
			playingRoundCount++;

			this->logHand();

			// state : preparing to play a round
			Card cardToSend = retrieveCardToSend();
			informCardHasBeenSelected();
			message = "readyToSendReceiveSemaphore.wait";
			Logger::getInstance()->logPlayer(this->number, message, INFO);
			readyToSendReceiveSemaphore.wait();

			// state : playing round
			cardToSendMemory.setCard(cardToSend);
			senderSemaphore.signal();
			message = "senderSemaphore.signal";
			Logger::getInstance()->logPlayer(this->number, message, INFO);
			receiverSemaphore.signal();
			message = "receiverSemaphore.signal";
			Logger::getInstance()->logPlayer(this->number, message, INFO);
			message = "sentSemaphore.wait";
			Logger::getInstance()->logPlayer(this->number, message, INFO);
			sentSemaphore.wait();
			message = "receivedSemaphore.wait";
			Logger::getInstance()->logPlayer(this->number, message, INFO);
			receivedSemaphore.wait();
			Card receivedCard = receivedCardMemory.getCard();
			this->hand.push_back(receivedCard);
			message = "Received card:" + receivedCard.toString();
			Logger::getInstance()->logPlayer(this->number, message, INFO);

			// state: checking if i win
			if (isWinningHand())
			{
				{
					Lock l(NamingService::getDealingFifoName(this->number));
					if (this->playingRound){
						this->playingRound = false;
						message = "Player put down hand (winner)";
						Logger::getInstance()->logPlayer(this->number, message, INFO);
						informMyHandIsOnTheTable();
					}
				}
			}
		}
	}

	message = "Head - Ending";
	Logger::getInstance()->logPlayer(this->number, message, INFO);
}

void PlayerHead::logHand(){
	string message = "Cards = ";
	for( int i = 0; i < 4 ; i++ )
	{
		message += this->hand[i].toString() + " | ";
	}
	Logger::getInstance()->logPlayer(this->number, message, INFO);
}

int PlayerHead::handleSignal (int signum){
	if (signum == SignalNumbers::PlayerWon){
		Lock l(NamingService::getDealingFifoName(this->number));
		if (this->playingRound){
			this->playingRound = false;
			string message = "Player put down hand (not winner)";
			Logger::getInstance()->logPlayer(this->number, message, INFO);
			informMyHandIsOnTheTable();
		}
	}
	else if (signum == SignalNumbers::GameOver){
		kill(this->receiverProcessId, SignalNumbers::GameOver);
		kill(this->senderProcessId, SignalNumbers::GameOver);
		this->gameOver = true;
	}
	else {
		return -1;
	}

	return 0;
}

PlayerHead::~PlayerHead() {
	string message = "Head - Destroying";
	Logger::getInstance()->logPlayer(this->number, message, INFO);
}
