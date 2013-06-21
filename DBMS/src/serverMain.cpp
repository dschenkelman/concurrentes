//#ifdef MAIN_SERVER

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <errno.h>

#include "Messages/MessageActions.h"
#include "Messages/MessageRequest.h"
#include "Messages/MessageResponse.h"

#include "DataBase/DataBaseManager.h"

#include "Concurrency/MessageQueue.h"

using namespace std;

#define READ_NEXT 0


bool isWorking = true;

MessageQueue<messageRequest> *mqRequest = NULL;
MessageQueue<messageResponse> *mqResponse = NULL;

bool prepareDataBase();
bool createMessageQueues();
struct messageRequest readRequest();
struct messageRequest readRequestWithId(int id);
std::list<struct messageResponse> processRequest(struct messageRequest request);
void sendResponse(std::list<struct messageResponse> responseStructures);

void prepareForGracefullQuit();
void saveDataBaseChanges();
void releaseMessageQueueResources();

struct messageRequest checkForAdministratorRequest();

int main(int argc, char **argv)
{

	bool error;

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

	/*int seconds = 10;
	cout << "Waiting " << seconds << " seconds for clients to enqueue" << endl;
	sleep(seconds);*/

	while( isWorking )
	{
		struct messageRequest request;
		std::list<struct messageResponse> responseStructures;

		request = checkForAdministratorRequest();
		if( NULL_ACTION_TYPE == request.requestActionType )
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


struct messageRequest checkForAdministratorRequest()
{
	struct messageRequest r;
	int result = mqRequest->readWithoutBlocking(1, &r);
	if( -1 == result )
		r.requestActionType = NULL_ACTION_TYPE;
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

		success = DataBaseManager::getInstance()->updatePerson(request.nameId, newPerson, true);
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
	case GRACEFUL_QUIT:
	{
		prepareForGracefullQuit();
		isWorking = false;
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


}

void prepareForGracefullQuit()
{
	cout << "Preparing for GracefullQuit" << endl;
	// remove temp files (this way if a client wants to open a conection it stops)
	remove(ASSET_REQUEST_FILE);
	remove(ASSET_RESPONSE_FILE);

	struct messageRequest request;
	std::list<struct messageResponse> eocResponses;
	while( -1 != mqRequest->readWithoutBlocking(0, &request) )
	{
		struct messageResponse eocResponse;
		eocResponse.clientId = request.clientId;
		cout << "ClientId: " << eocResponse.clientId << endl;
		eocResponse.responseActionType = ENDOFCONNECTION;
		eocResponse.numberOfRegisters = 0;
		eocResponses.push_back(eocResponse);
	}
	struct messageResponse eocResponse;
	eocResponse.clientId = 1;
	cout << "ClientId (the administrator): " << 1 << endl;
	eocResponse.responseActionType = ENDOFCONNECTION;
	eocResponse.numberOfRegisters = 0;
	eocResponses.push_back(eocResponse);
	sendResponse(eocResponses);
}

//#endif
