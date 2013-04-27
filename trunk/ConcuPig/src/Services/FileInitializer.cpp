/*
 * FileInitializer.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#include "FileInitializer.h"
#include "NamingService.h"

FileInitializer::FileInitializer(int players) : players(players) {
}

void FileInitializer::Initialize(){
	this->InitializeFifos();
}

void FileInitializer::InitializeFifos(){
	for(int i = 0; i < this->players; i++){
		NamingService::getCardPassingFifoKey(i, (i + 1) % this->players);
	}


}

FileInitializer::~FileInitializer() {
}
