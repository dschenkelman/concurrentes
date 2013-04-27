#include "Semaphore.h"

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

void Semaphore :: eliminate () {
	semctl ( this->id,0,IPC_RMID );
}
