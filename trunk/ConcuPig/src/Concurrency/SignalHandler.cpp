/*
 * SignalHandler.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#include "SignalHandler.h"

SignalHandler* SignalHandler :: instance = NULL;
IEventHandler* SignalHandler :: signal_handlers [ NSIG ];

SignalHandler :: SignalHandler () {
}

SignalHandler* SignalHandler :: getInstance () {

	if ( instance == NULL )
		instance = new SignalHandler();

	return instance;
}

void SignalHandler :: destroy () {
	if ( instance != NULL ) {
		delete ( instance );
		instance = NULL;
	}
}

IEventHandler* SignalHandler :: registerHandler ( int signum, IEventHandler* eh ) {
	IEventHandler* old_eh = SignalHandler :: signal_handlers [ signum ];
	SignalHandler :: signal_handlers [ signum ] = eh;

	struct sigaction sa;
	sa.sa_handler = SignalHandler :: dispatcher;
	sigemptyset ( &sa.sa_mask );	// initialized mask of signals to block
	sigaddset ( &sa.sa_mask,signum );
	sigaction ( signum,&sa,0 );	// update signal action

	return old_eh;
}

void SignalHandler :: dispatcher ( int signum ) {
	if ( SignalHandler :: signal_handlers [ signum ] != 0 )
		SignalHandler :: signal_handlers [ signum ]->handleSignal ( signum );
}

int SignalHandler :: removeHandler ( int signum ) {
	SignalHandler :: signal_handlers [ signum ] = NULL;
	return 0;
}
