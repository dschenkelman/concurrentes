#ifndef MESSAGERESPONSE_H_
#define MESSAGERESPONSE_H_

#define ASSET_RESPONSE_FILE "/tmp/response.file"

#include "../DataBase/PersonRegister.h"

typedef struct messageResponse {
	long clientId; //Clients priority lookup
	int answerActionType;

	int numberOfRegisters; //Used in head action type

	char name[SIZE_NAME];
	char direction[SIZE_DIRECTION];
	char telephone[SIZE_TELEPHONE];
} messageResponse;


#endif /* MESSAGERESPONSE_H_ */
