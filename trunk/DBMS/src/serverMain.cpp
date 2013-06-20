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
	cout << "Creating MQs" << endl;
	std::ofstream stream;
	stream.open(ASSET_REQUEST_FILE, ios::out | ios::app);
	stream.close();
	stream.open(ASSET_RESPONSE_FILE, ios::out | ios::app);
	stream.close();

	mqRequest = new MessageQueue<struct messageRequest>(ASSET_REQUEST_FILE);
	mqResponse = new MessageQueue<struct messageResponse>(ASSET_RESPONSE_FILE);

	cout << "MQs created" << endl;

	return (NULL == mqRequest || NULL == mqResponse) ;
}


struct messageRequest readRequest()
{
	cout << "Reading next Request" << endl;
	struct messageRequest r;
	r = readRequestWithId(READ_NEXT);
	cout << "Request Type: "<< r.requestActionType << endl;
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
	cout << "Processing Request" << endl;
	std::list<struct messageResponse> r;
	struct person newPerson;
	strcpy(newPerson.name, request.name);
	strcpy(newPerson.address, request.address);
	strcpy(newPerson.telephone, request.telephone);

	switch(request.requestActionType)
	{
	case CREATE:
	{
		cout << "In create" << endl;
		bool success;
		int messageType;

		success = DataBaseManager::getInstance()->createPerson(newPerson);
		messageType = true == success ? OPERATION_CREATE_SUCCESS : OPERATION_FAILED;

		struct messageResponse head;
		head.clientId = request.clientId;
		head.responseActionType = messageType;
		head.numberOfRegisters = 0;
		strcpy(head.name, newPerson.name);
		strcpy(head.address, newPerson.address);
		strcpy(head.telephone, newPerson.telephone);

		r.push_back(head);
		break;
	}
	case UPDATE:
	{
		cout << "In update" << endl;
		bool success;
		int messageType;

		success = DataBaseManager::getInstance()->updatePerson(newPerson, true);
		messageType = true == success ? OPERATION_UPDATE_SUCCESS : OPERATION_FAILED;

		struct messageResponse head;
		head.clientId = request.clientId;
		head.responseActionType = messageType;
		head.numberOfRegisters = 0;
		strcpy(head.name, newPerson.name);
		strcpy(head.address, newPerson.address);
		strcpy(head.telephone, newPerson.telephone);

		r.push_back(head);
		break;
	}
	case READ:
	{
		cout << "In read" << endl;
		std::list<struct person> persons = DataBaseManager::getInstance()->retrievePersons(
				request.name, request.address, request.telephone);
		std::list<struct person>::iterator it = persons.begin();

		struct messageResponse head;
		head.clientId = request.clientId;
		head.responseActionType = HEAD;
		head.numberOfRegisters = persons.size();
		strcpy(head.name, newPerson.name);
		strcpy(head.address, newPerson.address);
		strcpy(head.telephone, newPerson.telephone);

		r.push_back(head);

		while( persons.end() != it )
		{
			struct messageResponse body;
			body.clientId = request.clientId;
			body.responseActionType = BODY;
			body.numberOfRegisters = 0;
			strcpy(body.name, it->name);
			strcpy(body.address, it->address);
			strcpy(body.telephone, it->telephone);

			r.push_back(body);
			it++;
		}
		break;
	}
	case DELETE:
	{
		cout << "In delete" << endl;
		bool success;
		int messageType;

		success = DataBaseManager::getInstance()->deletePerson(newPerson);
		messageType = true == success ? OPERATION_DELETE_SUCCESS : OPERATION_FAILED;

		struct messageResponse head;
		head.clientId = request.clientId;
		head.responseActionType = messageType;
		head.numberOfRegisters = 0;
		strcpy(head.name, newPerson.name);
		strcpy(head.address, newPerson.address);
		strcpy(head.telephone, newPerson.telephone);

		r.push_back(head);
		break;
	}
	default :
	{
		cout << "In default" << endl;
		struct messageResponse head;
		head.clientId = request.clientId;
		head.responseActionType = OPERATION_UNKNOWN;
		head.numberOfRegisters = 0;
		strcpy(head.name, newPerson.name);
		strcpy(head.address, newPerson.address);
		strcpy(head.telephone, newPerson.telephone);

		r.push_back(head);
		break;
	}
	}
	return r;
}

void sendResponse(std::list<struct messageResponse> responseStructures)
{
	cout << "Sending Response (Count: "<< responseStructures.size()<<")" << endl;
	std::list<struct messageResponse>::iterator it =
		responseStructures.begin();
	while(responseStructures.end() != it)
	{
		mqResponse->write(*it);
		it++;
	}
}

void saveDataBaseChanges()
{
	cout << "Saving db changes" << endl;
	DataBaseManager::getInstance()->finalize();
}

void releaseMessageQueueResources()
{
	cout << "Releasing MQs resources" << endl;
	mqRequest->destroy();
	mqResponse->destroy();
	delete(mqRequest);
	delete(mqResponse);

	remove(ASSET_REQUEST_FILE);
	remove(ASSET_RESPONSE_FILE);
}
#endif
