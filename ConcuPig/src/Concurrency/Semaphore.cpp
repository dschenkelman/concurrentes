#include "Semaphore.h"
#include "../Services/Logger.h"
#include <string>
#include <errno.h>
#include "../Helpers/Convert.h"

using namespace std;

Semaphore :: Semaphore (key_t key) {
	this->id = semget ( key,1,0666 | IPC_CREAT );
}

Semaphore::~Semaphore() {
}

int Semaphore :: initialize (int initialValue) {

	union semnum {
		int val;
		struct semid_ds* buf;
		ushort* array;
	};

	semnum init;
	init.val = initialValue;
	int result = semctl ( this->id,0,SETVAL,init );

	if (result == -1){
		string message = "Semaphore initialize failed";
		Logger::getInstance()->logLine(message, ERROR);
	}

	return result;
}

int Semaphore :: wait () {

	struct sembuf operation;

	operation.sem_num = 0;	// Semaphore number
	operation.sem_op  = -1;	// Decrease by one the semaphore
	operation.sem_flg = SEM_UNDO;

	int result = semop ( this->id,&operation, 1);

	if (result == -1){
		int e = errno;
		string message = "Semaphore wait failed:" + Convert::toString(e);
		Logger::getInstance()->logLine(message, ERROR);
	}
	return result;
}

int Semaphore :: signal () {

	struct sembuf operation;

	operation.sem_num = 0;	// Semaphore number
	operation.sem_op  = 1;	// Increase by one the semaphore
	operation.sem_flg = SEM_UNDO;

	int result = semop (this->id, &operation, 1);

	if (result == -1){
		int e = errno;
		string message = "Semaphore signal failed:" + Convert::toString(e);
		Logger::getInstance()->logLine(message, ERROR);
	}

	return result;
}

void Semaphore :: eliminate () {
	string message = "Semaphore eliminated";
	Logger::getInstance()->logLine(message, WARNING);
	semctl ( this->id,0,IPC_RMID, NULL);
}
