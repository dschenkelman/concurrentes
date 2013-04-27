/*
 * IEventHandler.h
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#ifndef IEVENTHANDLER_H_
#define IEVENTHANDLER_H_

class IEventHandler {

public:
	virtual int handleSignal ( int signum ) = 0;
	virtual ~IEventHandler () {};
};

#endif /* IEVENTHANDLER_H_ */
