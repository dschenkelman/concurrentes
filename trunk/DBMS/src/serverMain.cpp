#ifdef MAIN_SERVER

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <list>

#include "Messages/MessageActions.h"
#include "Messages/MessageRequest.h"
#include "Messages/MessageResponse.h"

#include "DataBase/DataBaseManager.h"

#include "Concurrency/MessageQueue.h"

using namespace std;

#define READ_NEXT 0


MessageQueue<messageRequest> *mqRequest = NULL;
MessageQueue<messageResponse> *mqResponse = NULL;

bool prepareDataBase();
bool createMessageQueues();
struct messageRequest readRequest();
struct messageRequest readRequestWithId(int id);
std::list<struct messageResponse> processRequest(struct messageRequest request);
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
		if( NULL != mqRequest )
		{
			mqRequest->destroy();
			delete( mqRequest );
		}
		if( NULL != mqResponse )
		{
			mqResponse->destroy();
			delete( mqResponse );
		}
		exit(1);
	}

	isWorking = true;
	while( isWorking )
	{
		struct messageRequest request;
		std::list<struct messageResponse> responseStructures;

		request = readRequest();
		responseStructures = processRequest(request);
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
	std::ofstream stream;
	stream.open(ASSET_REQUEST_FILE, ios::out | ios::app);
	stream.close();
	stream.open(ASSET_RESPONSE_FILE, ios::out | ios::app);
	stream.close();

	mqRequest = new MessageQueue<struct messageRequest>(ASSET_REQUEST_FILE, 'a');
	mqResponse = new MessageQueue<struct messageResponse>(ASSET_RESPONSE_FILE, 'a');

	return NULL == mqRequest || NULL == mqResponse;
}


struct messageRequest readRequest()
{
	struct messageRequest r;
	r = readRequestWithId(READ_NEXT);
	return r;
}

struct messageRequest readRequestWithId(int id)
{
	struct messageRequest r;
	mqRequest->read(id, &r);
	return r;
}

std::list<struct messageResponse> processRequest(struct messageRequest request)
{
	std::list<struct messageResponse> r;
	struct person newPerson;
	strcpy(newPerson.name, request.name);
	strcpy(newPerson.direction, request.direction);
	strcpy(newPerson.telephone, request.telephone);

	switch(request.petitionActionType)
	{
	case CREATE:
	{
		// TODO create head
		DataBaseManager::getInstance()->createPerson(newPerson);
		break;
	}
	case UPDATE:
	{
		// TODO create head
		DataBaseManager::getInstance()->updatePerson(newPerson, true);
		break;
	}
	case READ:
	{
		// TODO create head
		std::list<struct person> persons = DataBaseManager::getInstance()->retrievePersons(
				request.name, request.direction, request.telephone);
		break;
	}
	case DELETE:
	{
		// TODO create head
		DataBaseManager::getInstance()->deletePerson(newPerson);
		break;
	}
	default :
	{
		// TODO create head for unknown action
		break;
	}
	}
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
