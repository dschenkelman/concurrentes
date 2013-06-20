#ifdef MAIN_SERVER

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
/*
	cout << "Initial data" << endl;
	std::list<struct person> list = DataBaseManager::getInstance()->retrievePersons("*", "*", "*");
	std::list<struct person>::iterator it = list.begin();
	while( list.end() != it )
	{
		cout << it->name << "," << it->address << "," << it->telephone << endl;
		it++;
	}

	cout << endl <<"Modified Data:" << endl;

	struct person person;
	strcpy(person.name, "Matias Servetto");
	strcpy(person.address, "El Talar - Ecuador 871");
	strcpy(person.telephone, "1170707060");
	DataBaseManager::getInstance()->updatePerson(person, true);
	strcpy(person.name, "Sebastian Rodriguez");
	strcpy(person.address, "Ugart");
	strcpy(person.telephone, "120");
	DataBaseManager::getInstance()->updatePerson(person, true);
	strcpy(person.name, "MOMO");
	strcpy(person.address, "aasasa");
	strcpy(person.telephone, "asajksa");
	DataBaseManager::getInstance()->updatePerson(person, false);
	strcpy(person.name, "POPO");
	strcpy(person.address, "aasasa");
	strcpy(person.telephone, "asajksa");
	DataBaseManager::getInstance()->updatePerson(person, true);

	list = DataBaseManager::getInstance()->retrievePersons("*", "*", "*");
	it = list.begin();
	while( list.end() != it )
	{
		cout << it->name << "," << it->address << "," << it->telephone << endl;
		it++;
	}

	cout << endl <<"Filtered Data:" << endl;

	list = DataBaseManager::getInstance()->retrievePersons("*", "*", "*");
	it = list.begin();
	while( list.end() != it )
	{
		cout << it->name << "," << it->address << "," << it->telephone << endl;
		it++;
	}
*/

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
#endif
