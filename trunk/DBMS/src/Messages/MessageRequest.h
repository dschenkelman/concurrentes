#ifndef MESSAGEREQUEST_H_
#define MESSAGEREQUEST_H_

#define ASSET_REQUEST_FILE "/tmp/request.file"

#define SIZE_NAME 61
#define SIZE_DIRECTION 120
#define SIZE_TELEPHONE 13

typedef struct messageRequest{
	long clientId;
	int petitionActionType;

	char name[SIZE_NAME];
	char direction[SIZE_DIRECTION];
	char telephone[SIZE_TELEPHONE];

	char nameId[SIZE_NAME]; //Used for Update/Delete searches
} messageRequest;


#endif /* MESSAGEREQUEST H_ */
