/*
 * SharedMemoryNames.h
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#ifndef SHAREDMEMORYNAMES_H_
#define SHAREDMEMORYNAMES_H_

#include <string>

class SharedMemoryNames {
public:
	static const std::string CardToSend;
	static const std::string CardToReceive;
	static const std::string Scoreboard;
	static const std::string Score;
};

#endif /* SHAREDMEMORYNAMES_H_ */
