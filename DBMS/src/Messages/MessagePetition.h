#ifndef MESSAGEPETITION_H_
#define MESSAGEPETITION_H_

#define SIZE_NAME 61
#define SIZE_DIRECTION 120
#define SIZE_TELEPHONE 13

typedef struct messagePetition {
	long clientId;
	int petitionActionType;

	char name[SIZE_NAME];
	char direction[SIZE_DIRECTION];
	char telephone[SIZE_TELEPHONE];

	char nameId[SIZE_NAME]; //Used for Update/Delete searches
} messagePetition;


#endif /* MESSAGEPETITION_H_ */
