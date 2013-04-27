/*
 * SignalHandler.h
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#ifndef SIGNALHANDLER_H_
#define SIGNALHANDLER_H_

#include <signal.h>
#include <stdio.h>

#include "IEventHandler.h"

class SignalHandler {
	private:
		static SignalHandler* instance;
		static IEventHandler* signal_handlers [ NSIG ];
		SignalHandler ( void );
		static void dispatcher ( int signum );
	public:
		static SignalHandler* getInstance ();
		static void destroy();
		IEventHandler* registerHandler (int signum, IEventHandler* eh);
		int removeHandler (int signum);
};


#endif /* SIGNALHANDLER_H_ */
