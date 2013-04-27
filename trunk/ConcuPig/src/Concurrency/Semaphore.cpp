#include "Semaphore.h"

using namespace std;

Semaphore :: Semaphore ( key_t key, int initialValue ) {

	this->initialValue = initialValue;

	this->id = semget ( key,1,0666 | IPC_CREAT );

	this->initialize ();
}

Semaphore::~Semaphore() {
}

int Semaphore :: initialize () {

	union semnum {
		int val;
		struct semid_ds* buf;
		ushort* array;
	};

	semnum init;
	init.val = this->initialValue;
	int result = semctl ( this->id,0,SETVAL,init );
	return result;
}

int Semaphore :: wait () {

	struct sembuf operation;

	operation.sem_num = 0;	// Semaphore number
	operation.sem_op  = -1;	// Decrease by one the semaphore
	operation.sem_flg = SEM_UNDO;

	int result = semop ( this->id,&operation,1 );
	return result;
}

int Semaphore :: signal () {

	struct sembuf operation;

	operation.sem_num = 0;	// Semaphore number
	operation.sem_op  = 1;	// Increase by one the semaphore
	operation.sem_flg = SEM_UNDO;

	int result = semop ( this->id,&operation,1 );
	return result;
}

void Semaphore :: deleteSemaphore () {
	semctl ( this->id,0,IPC_RMID );
}
