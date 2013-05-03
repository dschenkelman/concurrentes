/*
 * SemaphoreNames.h
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#ifndef SEMAPHORENAMES_H_
#define SEMAPHORENAMES_H_

#include <string>

class SemaphoreNames {
public:
	static const std::string DealtSemaphore;
	static const std::string ReadyToSendReceive;
	static const std::string ReceiverSemaphore;
	static const std::string ReceivedSemaphore;
	static const std::string SenderSemaphore;
	static const std::string SentSemaphore;
};

#endif /* SEMAPHORENAMES_H_ */
