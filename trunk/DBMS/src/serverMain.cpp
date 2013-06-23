#ifdef MAIN_SERVER

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <errno.h>
#include <sys/wait.h>

#include "Messages/MessageActions.h"
#include "Messages/MessageRequest.h"
#include "Messages/MessageResponse.h"

#include "DataBase/DataBaseManager.h"

#include "Concurrency/MessageQueue.h"

#include "Helpers/Logger.h"
#include "Helpers/Convert.h"

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
		Logger::getInstance();

		bool error;

		error = prepareDataBase();
		if( true == error )
		{
			Logger::getInstance()->logMessage("prepareDataBase() error");
			Logger::getInstance()->terminate();
			exit(1);
		}

		error = createMessageQueues();
		if( true == error )
		{
			Logger::getInstance()->logMessage("createMessageQueues() error");
			Logger::getInstance()->terminate();
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
			std::string message = "Waiting ";
			message += Convert::toString(seconds);
			message += " seconds for clients to enqueue";
			Logger::getInstance()->logMessage(message);
			sleep(seconds);
		}

		while( isWorking )
		{
			struct messageRequest request;
			std::list<struct messageResponse> responseStructures;

			request = checkForAdministratorRequest();
			if( NULL_ACTION_TYPE == request.requestActionType )
				request = readRequest();
			Logger::getInstance()->logRequest(request);
			responseStructures = processRequest(request);
			sendResponse(responseStructures);
		}

		saveDataBaseChanges();
		releaseMessageQueueResources();

		Logger::getInstance()->logMessage("Gracefully quited");
		Logger::getInstance()->terminate();

		exit( 0 );
	}
	else
	{
		cout << "Enter any key to stop the server" << endl;

		int character = fgetc(stdin);

		pid = fork();
		if( 0 == pid )
		{
			char gracefulQuit[6];
			snprintf(gracefulQuit, sizeof(gracefulQuit), "%d", GRACEFUL_QUIT);

			char *execArgs[] = {
					"./admin",
					gracefulQuit,
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
	Logger::getInstance()->logMessage("Creating MQs");
	std::ofstream stream;
	stream.open(ASSET_REQUEST_FILE, ios::out | ios::app);
	stream.close();
	stream.open(ASSET_RESPONSE_FILE, ios::out | ios::app);
	stream.close();

	mqRequest = new MessageQueue<struct messageRequest>(ASSET_REQUEST_FILE);
	mqResponse = new MessageQueue<struct messageResponse>(ASSET_RESPONSE_FILE);

	Logger::getInstance()->logMessage("MQs created");

	return (NULL == mqRequest || NULL == mqResponse) ;
}


struct messageRequest readRequest()
{
	Logger::getInstance()->logMessage("Reading next Request");
	struct messageRequest request;
	request = readRequestWithId(READ_NEXT);
	Logger::getInstance()->logMessage("Request Type : " + Convert::toString(request.requestActionType));
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
	Logger::getInstance()->logMessage("Processing Request");
	std::list<struct messageResponse> messageResponseList;
	struct person newPerson;
	strcpy(newPerson.name, request.name);
	strcpy(newPerson.address, request.address);
	strcpy(newPerson.telephone, request.telephone);

	switch(request.requestActionType)
	{
	case CREATE:
	{
		int messageType;

		messageType = DataBaseManager::getInstance()->createPerson(newPerson);

		struct messageResponse head;
		head.clientId = request.clientId;
		head.responseActionType = messageType;
		head.numberOfRegisters = 0;
		strcpy(head.name, newPerson.name);
		strcpy(head.address, newPerson.address);
		strcpy(head.telephone, newPerson.telephone);

		messageResponseList.push_back(head);
		break;
	}
	case UPDATE:
	{
		int messageType;

		messageType = DataBaseManager::getInstance()->updatePerson(request.nameId, newPerson, false);

		struct messageResponse head;
		head.clientId = request.clientId;
		head.responseActionType = messageType;
		head.numberOfRegisters = 0;
		strcpy(head.name, newPerson.name);
		strcpy(head.address, newPerson.address);
		strcpy(head.telephone, newPerson.telephone);

		messageResponseList.push_back(head);
		break;
	}
	case READ:
	{
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

		messageResponseList.push_back(head);

		while( persons.end() != it )
		{
			struct messageResponse body;
			body.clientId = request.clientId;
			body.responseActionType = BODY;
			body.numberOfRegisters = 0;
			strcpy(body.name, it->name);
			strcpy(body.address, it->address);
			strcpy(body.telephone, it->telephone);

			messageResponseList.push_back(body);
			it++;
		}
		break;
	}
	case DELETE:
	{
		bool success;
		int messageType;

		messageType = DataBaseManager::getInstance()->deletePerson(newPerson);

		struct messageResponse head;
		head.clientId = request.clientId;
		head.responseActionType = messageType;
		head.numberOfRegisters = 0;
		strcpy(head.name, newPerson.name);
		strcpy(head.address, newPerson.address);
		strcpy(head.telephone, newPerson.telephone);

		messageResponseList.push_back(head);
		break;
	}
	case GRACEFUL_QUIT:
	{
		messageResponseList = prepareForGracefullQuit();
		isWorking = false;
		break;
	}
	default :
	{
		Logger::getInstance()->logMessage("Request Activity Type Unknown");
		struct messageResponse head;
		head.clientId = request.clientId;
		head.responseActionType = OPERATION_UNKNOWN;
		head.numberOfRegisters = 0;
		strcpy(head.name, newPerson.name);
		strcpy(head.address, newPerson.address);
		strcpy(head.telephone, newPerson.telephone);

		messageResponseList.push_back(head);
		break;
	}
	}
	return messageResponseList;
}

void sendResponse(std::list<struct messageResponse> responseStructures)
{
	std::string message = "Sending Response (Count: ";
	message += Convert::toString(responseStructures.size());
	message += ")";
	Logger::getInstance()->logMessage(message);
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
	Logger::getInstance()->logMessage("Saving db changes");
	DataBaseManager::getInstance()->finalize();
}

void releaseMessageQueueResources()
{
	Logger::getInstance()->logMessage("Releasing MQs resources");
	mqRequest->destroy();
	mqResponse->destroy();
	delete(mqRequest);
	delete(mqResponse);
	Logger::getInstance()->logMessage("Releasing finished");


}

std::list<struct messageResponse> prepareForGracefullQuit()
{
	Logger::getInstance()->logMessage("Preparing for GracefullQuit");
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
		Logger::getInstance()->logMessage("Message in queue from ClientId: " + eocResponse.clientId);
		eocResponse.responseActionType = ENDOFCONNECTION;
		eocResponse.numberOfRegisters = 0;
		eocResponses.push_back(eocResponse);
	}
	// inform the administrator that server has gracefully quit
	struct messageResponse eocResponse;
	eocResponse.clientId = 1;
	Logger::getInstance()->logMessage("ClientId (the administrator): 1" );
	eocResponse.responseActionType = ENDOFCONNECTION;
	eocResponse.numberOfRegisters = 0;
	eocResponses.push_back(eocResponse);

	return eocResponses;
}

#endif
