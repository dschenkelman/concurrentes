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

PlayerHead::PlayerHead( int playerNumber, int leftPlayerNumber, int rightPlayerNumber) :
	number(playerNumber),
	leftPlayerNumber(leftPlayerNumber),
	rightPlayerNumber(rightPlayerNumber),
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
	createSubProcess();
}

void PlayerHead::createSubProcess()
{
	pid_t id = fork ();
	if ( id == 0 )
	{
		//cout << "Hijo: Hola, soy el proceso hijo.  Mi process ID es " << getpid() << endl;
		//cout << "Hijo: El process ID de mi padre es " << getppid() << endl;
		PlayerCardReceiver receiver(this->number, this->leftPlayerNumber);
		std::string message = "Receiver created";
		Logger::getInstance()->logPlayer(this->number, message, INFO);
		receiver.run();
	} else
	{
		//cout << "Padre: Hola, soy el proceso padre.  Mi process ID es " << getpid() << endl;
		//cout << "Padre: El process ID de mi hijo es " << id << endl;
		id = fork ();
		if ( id == 0 )
		{
			//cout << "Hijo: Hola, soy el proceso hijo.  Mi process ID es " << getpid() << endl;
			//cout << "Hijo: El process ID de mi padre es " << getppid() << endl;
			PlayerCardSender sender(this->number, this->rightPlayerNumber);
			std::string message = "Sender created";
			Logger::getInstance()->logPlayer(this->number, message, INFO);
			sender.run();
		}
	}
	std::string message = "Head created";
	Logger::getInstance()->logPlayer(this->number, message, INFO);
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
	std::string message = "playerReadyFifo.writeValue";
	Logger::getInstance()->logPlayer(this->number, message, INFO);
	this->playerReadyFifo.writeValue((char*)this->number, sizeof(int));
}

void PlayerHead::informMyHandIsOnTheTable()
{
	std::string message = "handDownFifo.writeValue";
	Logger::getInstance()->logPlayer(this->number, message, INFO);
	this->handDownFifo.writeValue((char*)this->number, sizeof(int));
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
	while( true )
	{
		std::string message = "dealtSemaphore.wait";
		Logger::getInstance()->logPlayer(this->number, message, INFO);

		for( int i = 0 ; i < 4 ; i++)
		{
			char serializedCard[2];
			message = "dealtFifo.readValue";
			Logger::getInstance()->logPlayer(this->number, message, INFO);
			dealtFifo.readValue(serializedCard, 2);
			this->hand.push_back(Card(serializedCard[0], serializedCard[1]));
		}

		dealtSemaphore.wait();

		bool playingRound = true;
/*
		if( isWinningHand() )
		{
			informMyHandIsOnTheTable();
			playingRound = false;
		}
		*/


		while( playingRound )
		{
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

			// state: checking if i win
			if( isWinningHand() )
			{
				informMyHandIsOnTheTable();
				message = "informMyHandIsOnTheTable";
				Logger::getInstance()->logPlayer(this->number, message, INFO);
				playingRound = false;
			}
		}
	}
}

PlayerHead::~PlayerHead() { }

