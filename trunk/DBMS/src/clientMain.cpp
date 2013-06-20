//#ifdef MAIN_CLIENT

#include <iostream>
#include <stdlib.h>
#include <list>
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
int sendMessageRequest(struct messageRequest message, MessageQueue<struct messageRequest>* mqRequest);
bool readMessageResponse(int clientId, MessageQueue<struct messageResponse>* mqResponse, list<struct messageResponse>* vMessagesResponses);
void printMessageResponse(int requestAction,list<struct messageResponse>* vMessagesResponses);

using namespace std;

int main(int argc, char* argv[]) {
	int clientId = 1;
	messageRequest messageReq;

	if (! packageMessageRequest(clientId,argc,argv,&messageReq)){
		perror("packageMessageRequest failed");
		exit(1);
	}

	if( checkConnectionWithServer() ){

		MessageQueue<messageRequest> mqRequest(ASSET_REQUEST_FILE);
		MessageQueue<messageResponse> mqResponse(ASSET_RESPONSE_FILE);

		if (sendMessageRequest(messageReq,&mqRequest) == 0){

			list<struct messageResponse> vMessagesResponses;
			if ( readMessageResponse(clientId,&mqResponse,&vMessagesResponses)){
				printMessageResponse(messageReq.requestActionType,&vMessagesResponses);
			}else{
				perror("ReadMessageResponse failed");
				exit(1);
			}

		}else{
			perror("SendMessageRequest failed");
			exit(1);
		}

	}else{
		perror("Connection with server couldn't be established");
		exit(1);
	}



	return 0;
}

bool packageMessageRequest(int clientId,int parametersSize, char* petitionParameters[], struct messageRequest* message){

	if ( parametersSize < 5 ){
		return false;
	}else{
		message->clientId = clientId;
		message->requestActionType = Convert::toInt(petitionParameters[1]);
		strcpy ( message->name, petitionParameters[2] );
		strcpy ( message->address, petitionParameters[3] );
		strcpy ( message->telephone, petitionParameters[4] );
		if ( parametersSize > 5 ){
			strcpy ( message->nameId, petitionParameters[5] );
		}else{
			strcpy ( message->nameId, "" );
		}

		return true;
	}
}

bool checkConnectionWithServer(){
	//Check existence of files
	struct stat bufferRequest, bufferResponse;
	return ( (stat (ASSET_REQUEST_FILE, &bufferRequest) == 0) && (stat (ASSET_RESPONSE_FILE, &bufferResponse) == 0) );
}

int sendMessageRequest(struct messageRequest message, MessageQueue<struct messageRequest>* mqRequest){
	return (mqRequest->write(message));
}

bool readMessageResponse(int clientId, MessageQueue<struct messageResponse>* mqResponse, list<struct messageResponse>* vMessagesResponses){

	messageResponse msgFirstResponse;
	mqResponse->read(clientId,&msgFirstResponse);

	switch(msgFirstResponse.responseActionType){
	case HEAD:

		for(int i = 0; i < msgFirstResponse.numberOfRegisters; i++){
			messageResponse msgResponse;
			mqResponse->read(clientId,&msgResponse);
			if ( msgResponse.responseActionType == ENDOFCONNECTION ){
				perror("Server has shut down");
				return false;
			}
			vMessagesResponses->push_back(msgResponse);
		}
		break;

	case ENDOFCONNECTION:
		perror("Server has shut down");
		return false;
	}

	return true;
}

void printMessageResponse(int requestAction, list<struct messageResponse>* vMessagesResponses){
	  cout << "Server Response:" << endl;
	  if ( requestAction == READ ){
		  cout << "Name \t Address \t Telephone" << endl;
		  for (list<struct messageResponse>::iterator it = vMessagesResponses->begin(); it != vMessagesResponses->end(); it++){

			  if ( it->responseActionType == OPERATION_FAILED){
				  cout << "Request operation failed" << endl;
			  }
			  cout << it->name << " \t " << it->address << " \t " << it->telephone << endl;
		  }
	  }else{
		  if ( vMessagesResponses->begin()->responseActionType == OPERATION_FAILED){
			  cout << "Request operation failed" << endl;
		  }else{
			  cout << "Request operation completed successfully" << endl;
		  }

	  }


}

//#endif
