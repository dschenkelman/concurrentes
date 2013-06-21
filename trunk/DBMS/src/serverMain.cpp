//#ifdef MAIN_SERVER

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <errno.h>
#include <sys/wait.h>

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

std::list<struct messageResponse> prepareForGracefullQuit();
void saveDataBaseChanges();
void releaseMessageQueueResources();

struct messageRequest checkForAdministratorRequest();

int main(int argc, char **argv)
{
	int seconds = 0;
	int pid = fork();

	if( 0 == pid )
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
			remove(ASSET_REQUEST_FILE);
			remove(ASSET_RESPONSE_FILE);

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

		if( 0 != seconds )
		{
			cout << "Waiting " << seconds << " seconds for clients to enqueue" << endl;
			sleep(seconds);
		}

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

		exit( 0 );
	}
	else
	{
		cout << "Enter any key to stop the server" << endl;

		int character = fgetc(stdin);

		pid = fork();
		if( 0 == pid )
		{
			//execl("./admin", "13", "-a", "-a", "-a", (char *)0);
			char *execArgs[] = {
					"./admin",
					"13",
					"-a",
					"-a",
					"-a",
					0
			};
			execv("./admin", execArgs);
		}
		else
		{
			wait( NULL );
			exit( 0 );
		}

	}
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
	struct messageRequest request;
	request = readRequestWithId(READ_NEXT);
	cout << "Request Type: "<< request.requestActionType << endl;
	return request;
}

struct messageRequest readRequestWithId(int id)
{
	struct messageRequest request;
	mqRequest->read(id, &request);
	return request;
}


struct messageRequest checkForAdministratorRequest()
{
	struct messageRequest adminRequest;
	int result = mqRequest->readWithoutBlocking(1, &adminRequest);
	if( -1 == result )
		adminRequest.requestActionType = NULL_ACTION_TYPE;
	return adminRequest;
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
		r = prepareForGracefullQuit();
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

std::list<struct messageResponse> prepareForGracefullQuit()
{
	cout << "Preparing for GracefullQuit" << endl;
	// remove temp files (this way if a client wants to open a conection it stops)
	remove(ASSET_REQUEST_FILE);
	remove(ASSET_RESPONSE_FILE);

	struct messageRequest request;
	std::list<struct messageResponse> eocResponses;
	while( -1 != mqRequest->readWithoutBlocking(0, &request) )
	{
		// Prepare response for processes who are waiting for a response
		struct messageResponse eocResponse;
		eocResponse.clientId = request.clientId;
		cout << "ClientId: " << eocResponse.clientId << endl;
		eocResponse.responseActionType = ENDOFCONNECTION;
		eocResponse.numberOfRegisters = 0;
		eocResponses.push_back(eocResponse);
	}
	// inform the administrator that server has gracefully quit
	struct messageResponse eocResponse;
	eocResponse.clientId = 1;
	cout << "ClientId (the administrator): " << 1 << endl;
	eocResponse.responseActionType = ENDOFCONNECTION;
	eocResponse.numberOfRegisters = 0;
	eocResponses.push_back(eocResponse);

	return eocResponses;
}

//#endif
