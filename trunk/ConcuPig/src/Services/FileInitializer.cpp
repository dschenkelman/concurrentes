/*
 * FileInitializer.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#include "FileInitializer.h"
#include "NamingService.h"
#include "../Constants/SharedMemoryNames.h"

#include <string>

using namespace std;

FileInitializer::FileInitializer(int players) : players(players) {
}

void FileInitializer::Initialize(){
	this->InitializeSharedMemories();
}

void FileInitializer::InitializeSharedMemories(){
	for (int i = 0; i < this->players; i++){
		string receiveCardName = NamingService::getSharedCardFileName(SharedMemoryNames::CardToReceive, i);
		string sendCardName = NamingService::getSharedCardFileName(SharedMemoryNames::CardToSend, i);
	}
}

FileInitializer::~FileInitializer() {
}
