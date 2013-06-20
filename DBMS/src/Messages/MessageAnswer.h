#ifndef MESSAGEANSWER_H_
#define MESSAGEANSWER_H_

#define SIZE_NAME 61
#define SIZE_DIRECTION 120
#define SIZE_TELEPHONE 13

typedef struct messageAnswer {
	long clientId; //Clients priority lookup
	int answerActionType;

	int numberOfRegisters; //Used in head action type

	char name[SIZE_NAME];
	char direction[SIZE_DIRECTION];
	char telephone[SIZE_TELEPHONE];
} messageAnswer;


#endif /* MESSAGEANSWER_H_ */
