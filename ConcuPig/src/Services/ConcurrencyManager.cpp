/*
 * FileInitializer.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#include "ConcurrencyManager.h"
#include "NamingService.h"
#include "../Constants/SharedMemoryNames.h"
#include "../Constants/SemaphoreNames.h"
#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "../Services/Logger.h"

using namespace std;

ConcurrencyManager::ConcurrencyManager(int players) : players(players) {
}

void ConcurrencyManager::initialize(){
	this->initializeSharedMemories();
	this->initializeSemaphores();
	this->initializeFifos();
}

void ConcurrencyManager::initializeSharedMemories(){
	for (int i = 0; i < this->players; i++){
		string receiveCardName = NamingService::getSharedCardFileName(SharedMemoryNames::CardToReceive, i);
		string sendCardName = NamingService::getSharedCardFileName(SharedMemoryNames::CardToSend, i);

		this->createFile(receiveCardName);
		this->createFile(sendCardName);
	}

	this->createFile(NamingService::getScoreboardFileName());

	string message = "Before creating scoreboard";

	Logger::getInstance()->logLine(message, INFO);
}

void ConcurrencyManager::initializeSemaphores(){
	this->createFile(SemaphoreNames::DealtSemaphore);
	this->createFile(SemaphoreNames::ReadyToSendReceive);
	this->createFile(SemaphoreNames::ReceivedSemaphore);
	this->createFile(SemaphoreNames::ReceiverSemaphore);
	this->createFile(SemaphoreNames::SenderSemaphore);
	this->createFile(SemaphoreNames::SentSemaphore);

	for (int i = 0; i < this->players; i++){
		this->initializeSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::DealtSemaphore, i), 0);
		this->initializeSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::ReadyToSendReceive, i), 0);
		this->initializeSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::ReceivedSemaphore, i), 0);
		this->initializeSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::ReceiverSemaphore, i), 0);
		this->initializeSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::SenderSemaphore, i), 0);
		this->initializeSemaphore(NamingService::getSemaphoreKey(SemaphoreNames::SentSemaphore, i), 0);
	}
}

void ConcurrencyManager::initializeFifos(){
	for (int i = 0; i < this->players; i++){
		this->openFifo(NamingService::getCardPassingFifoFileName(i, (i + 1) % this->players));
		this->openFifo(NamingService::getDealingFifoName(i));
	}

	this->openFifo(NamingService::getHandDownFifoName());
	this->openFifo(NamingService::getPlayersReadyFifoName());
}

void ConcurrencyManager::openFifo(const string& name){
	Fifo f(name);
	f.openRead(false);
	f.openWrite();
	this->fifos.push_back(f);
}

void ConcurrencyManager::initializeSemaphore(key_t key, int initialValue){
	Semaphore s(key);
	s.initialize(initialValue);
	this->semaphores.push_back(s);
}

void ConcurrencyManager::terminate(){
	for (unsigned int i = 0; i < this->semaphores.size(); i++) {
		this->semaphores[i].eliminate();
	}

	for (unsigned int i = 0; i < this->fifos.size(); i++) {
		this->fifos[i].closeFifo();
		this->fifos[i].eliminate();
	}
}

void ConcurrencyManager::createFile(const string& fileName){
	std::ofstream stream;
	stream.open(fileName.c_str(), ios::out | ios::app);
	stream.close();
}

ConcurrencyManager::~ConcurrencyManager() {
//	if (this->scoreboard != NULL){
//		delete this->scoreboard;
//		this->scoreboard = NULL;
//	}
}
