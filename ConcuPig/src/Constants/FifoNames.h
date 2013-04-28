/*
 * FifoNames.h
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#ifndef FIFONAMES_H_
#define FIFONAMES_H_

#include <string>

class FifoNames {
public:
	FifoNames();
	static const std::string CardPassing;
	static const std::string Dealing;
	static const std::string HandDown;
	static const std::string PlayersReady;
	virtual ~FifoNames();
};

#endif /* FIFONAMES_H_ */
