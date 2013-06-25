//#ifdef MAIN_CLIENT

#include <iostream>
#include <stdlib.h>
#include <list>
#include <map>
#include <string.h>
#include <sys/stat.h>

#include "Concurrency/MessageQueue.h"

#include "Messages/MessageActions.h"
#include "Messages/MessageRequest.h"
#include "Messages/MessageResponse.h"

#include "Helpers/Convert.h"

using namespace std;

bool packageMessageRequest(int clientId,int parametersSize, char* petitionParameters[], struct messageRequest* message);
bool checkConnectionWithServer();
int sendMessageRequest(struct messageRequest message );
bool readMessageResponse(int clientId, list<struct messageResponse>* vMessagesResponses);
void printMessageResponse(int requestAction,list<struct messageResponse>* vMessagesResponses);
void releaseMessageQueuesResources();
bool validateNotAsteriskPattern(char* data);

bool GracefulQuitRequested;
MessageQueue<messageRequest>* MQRequest = NULL;
MessageQueue<messageResponse>* MQResponse = NULL;

using namespace std;

int main(int argc, char* argv[]) {
	int clientId = getpid();
	messageRequest messageReq;

	if (! packageMessageRequest(clientId,argc,argv,&messageReq)){
		cout << "Packaging the MessageRequest failed" << endl;
		exit(1);
	}

	if( checkConnectionWithServer() ){

		MQRequest = new MessageQueue<struct messageRequest>(ASSET_REQUEST_FILE);
		MQResponse = new MessageQueue<struct messageResponse>(ASSET_RESPONSE_FILE);

		if (sendMessageRequest(messageReq) == 0){
			list<struct messageResponse> vMessagesResponses;
			if ( readMessageResponse(clientId,&vMessagesResponses)){
				if (!GracefulQuitRequested){
					printMessageResponse(messageReq.requestActionType,&vMessagesResponses);
				}
			}else{
				cout << "ReadMessageRequest failed" << endl;
			}
		}else{
			cout << "SendMessageRequest failed" << endl;
		}

	}else{
		cout << "Connection with server couldn't be established" << endl;
	}

	releaseMessageQueuesResources();

	return 0;
}

bool packageMessageRequest(int clientId,int parametersSize, char* petitionParameters[], struct messageRequest* message){

	if ( parametersSize < 3 ){
		return false;
	}else{
		message->clientId = Convert::toInt(petitionParameters[1]) == GRACEFUL_QUIT ? 1 : clientId;
		message->requestActionType = Convert::toInt(petitionParameters[1]);

		switch(message->requestActionType){
		case READ:
			if (parametersSize < 5){
				return false;
			}else{
				strcpy ( message->name, 0 == strcmp(petitionParameters[2],"-a") ? "*" : petitionParameters[2]);
				strcpy ( message->address, 0 == strcmp(petitionParameters[3],"-a") ? "*" : petitionParameters[3] );
				strcpy ( message->telephone, 0 == strcmp(petitionParameters[4],"-a") ? "*" : petitionParameters[4] );
				strcpy ( message->nameId, "" );
			}
			break;
		case CREATE:
			if (parametersSize < 5){
				return false;
			}else{
				if ( (validateNotAsteriskPattern(petitionParameters[2])) && (validateNotAsteriskPattern(petitionParameters[3]))
						&& (validateNotAsteriskPattern(petitionParameters[4])) ){
					strcpy ( message->name, petitionParameters[2]);
					strcpy ( message->address, petitionParameters[3] );
					strcpy ( message->telephone, petitionParameters[4] );
					strcpy ( message->nameId, "" );
				}else{
					return false;
				}
			}
			break;
		case UPDATE:
			if (parametersSize < 6){
				return false;
			}else{
				if ( (validateNotAsteriskPattern(petitionParameters[2])) && (validateNotAsteriskPattern(petitionParameters[3]))
						&& (validateNotAsteriskPattern(petitionParameters[4])) && (validateNotAsteriskPattern(petitionParameters[5])) ){
					strcpy ( message->name, petitionParameters[2]);
					strcpy ( message->address, petitionParameters[3] );
					strcpy ( message->telephone, petitionParameters[4] );
					strcpy ( message->nameId, petitionParameters[5] );
				}else{
					return false;
				}
			}
			break;
		case DELETE:
			if ( (validateNotAsteriskPattern(petitionParameters[2])) ){
				strcpy ( message->name, petitionParameters[2]);
				strcpy ( message->address, "" );
				strcpy ( message->telephone, "" );
				strcpy ( message->nameId, "" );
			}else{
				return false;
			}
			break;
		case GRACEFUL_QUIT:
			strcpy ( message->name, "");
			strcpy ( message->address, "" );
			strcpy ( message->telephone, "" );
			strcpy ( message->nameId, "" );
			break;
		}

		return true;
	}
}

bool validateNotAsteriskPattern(char* data){
	return (0 != strcmp(data,"-a"));
}

bool checkConnectionWithServer(){
	//Check existence of files
	struct stat bufferRequest, bufferResponse;
	return ( (stat (ASSET_REQUEST_FILE, &bufferRequest) == 0) && (stat (ASSET_RESPONSE_FILE, &bufferResponse) == 0) );
}

int sendMessageRequest(struct messageRequest message){
	GracefulQuitRequested = (message.requestActionType == GRACEFUL_QUIT);
	return (MQRequest->write(message));
}

bool readMessageResponse(int clientId, list<struct messageResponse>* vMessagesResponses){
	messageResponse msgFirstResponse;
	int idRead = GracefulQuitRequested ? 1 : clientId;

	int result = MQResponse->read(idRead,&msgFirstResponse);
	if ( -1 == result ){
		return false;
	}
	switch(msgFirstResponse.responseActionType){
	case HEAD:
		if (0 == msgFirstResponse.numberOfRegisters){
			vMessagesResponses->push_back(msgFirstResponse);
		}else{
			for(int i = 0; i < msgFirstResponse.numberOfRegisters; i++){
				messageResponse msgResponse;
				result = MQResponse->read(clientId,&msgResponse);
				if ( -1 == result ){
					return false;
				}
				vMessagesResponses->push_back(msgResponse);
				if ( (msgResponse.responseActionType == ENDOFCONNECTION) /*|| (msgResponse.responseActionType > NULL_ACTION_TYPE) */){
					break;
				}
			}
		}
		break;

	default:
		vMessagesResponses->push_back(msgFirstResponse);
		break;
	}

	return true;
}

void printMessageResponse(int requestAction, list<struct messageResponse>* vMessagesResponses){
	map<int, string> ResponsesStringTable;
	ResponsesStringTable[OPERATION_FAILED] = "The requested operation has failed";
	ResponsesStringTable[ENDOFCONNECTION] = "Server has shut down";
	ResponsesStringTable[OPERATION_CREATE_SUCCESS] = "Create operation was successful";
	ResponsesStringTable[OPERATION_CREATE_FAIL_PERSON_EXISTS] = "Create operation failed. The person already exists";
	ResponsesStringTable[OPERATION_UPDATE_SUCCESS] = "Update operation was successful";
	ResponsesStringTable[OPERATION_UPDATE_FAIL_PERSON_EXISTS] = "Update operation failed. The new name belongs to a person that already exists";
	ResponsesStringTable[OPERATION_UPDATE_FAIL_PERSON_NOT_EXISTS] = "Update operation failed. The person does not exists";
	ResponsesStringTable[OPERATION_DELETE_SUCCESS] = "Delete operation was successful";
	ResponsesStringTable[OPERATION_DELETE_FAIL_PERSON_NOT_EXISTS] = "Delete operation failed. The person does not exists";
	ResponsesStringTable[OPERATION_UNKNOWN] = "Unknown operation. Please retry";
	ResponsesStringTable[HEAD] = "No person registers matched";

	for (list<struct messageResponse>::iterator it = vMessagesResponses->begin(); it != vMessagesResponses->end(); it++){
		switch(it->responseActionType){
		case BODY: //Read response
			cout << it->name << endl;
			cout << "\t" << it->address << endl;
			cout << "\t" << it->telephone << endl;
			cout << endl;
			break;
		default:
			if ( 1 == ResponsesStringTable.count(it->responseActionType) ){
				cout << ResponsesStringTable[vMessagesResponses->begin()->responseActionType] << endl;
			}else{
				cout << ResponsesStringTable[OPERATION_UNKNOWN] << endl;
			}
			break;
		}
	}

}

void releaseMessageQueuesResources(){
	delete(MQRequest);
	delete(MQResponse);
}

//#endif
