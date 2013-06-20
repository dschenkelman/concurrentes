/*
 * serverMain.cpp
 *
 *  Created on: 20/06/2013
 *      Author: matias
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <list>

#include "Messages/MessageActions.h"
#include "Messages/MessageRequest.h"
#include "Messages/MessageResponse.h"

#include "DataBase/DataBaseManager.h"

using namespace std;

bool prepareDataBase();
bool createMessageQueues();
struct messageRequest readRequest();
std::list<std::string> processRequest(struct messageRequest request);
std::list<struct messageResponse> createResponse(int clientId, std::list<std::string> registers);
void sendResponse(std::list<struct messageResponse> responseStructures);
void saveDataBaseChanges();
void releaseMessageQueueResources();

int main(int argc, char **argv)
{

	bool error;
	bool isWorking;

	error = prepareDataBase();
	if( true == error )
	{
		cout << "prepareDataBase() error" << endl;
		exit(1);
	}

	error = createMessageQueues();
	if( true == error )
	{
		cout << "createMessageQueues() error" << endl;
		exit(1);
	}

	isWorking = true;
	while( isWorking )
	{
		struct messageRequest request;
		std::list<std::string> registers;
		std::list<struct messageResponse> responseStructures;

		request = readRequest();
		registers = processRequest(request);
		responseStructures = createResponse(request.clientId, registers);
		sendResponse(responseStructures);
	}

	saveDataBaseChanges();
	releaseMessageQueueResources();

	return 0;
}

bool prepareDataBase()
{
	bool success = DataBaseManager::getInstance()->initialize();

	std::list<struct person> list = DataBaseManager::getInstance()->retrievePersons("*", "*", "*");
	std::list<struct person>::iterator it = list.begin();
	while( list.end() != it )
	{
		cout << it->name << "," << it->direction << "," << it->telephone << endl;
		it++;
	}


	return !success;
}

bool createMessageQueues()
{
	return true;
}


struct messageRequest readRequest()
{
	struct messageRequest r;

	return r;
}

std::list<std::string> processRequest(struct messageRequest request)
{
	std::list<std::string> r;

	return r;
}
std::list<struct messageResponse> createResponse(int clientId, std::list<std::string> registers)
{
	std::list<struct messageResponse> r;

	return r;
}
void sendResponse(std::list<struct messageResponse> responseStructures)
{

}

void saveDataBaseChanges()
{

}

void releaseMessageQueueResources()
{

}
