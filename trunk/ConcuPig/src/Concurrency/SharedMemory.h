/*
 * SharedMemory.h
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#ifndef SHAREDMEMORY_H_
#define SHAREDMEMORY_H_

#define SHM_OK 0
#define	FTOK_ERROR -1
#define SHMGET_ERROR -2
#define	SHMAT_ERROR -3

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>

template <class T> class SharedMemory {

private:

	int 	shmId;
	T*		dataPointer;

	int attachedProcessesCount ();

public:

	SharedMemory ();
	~SharedMemory ();
	int create(key_t key );
	void release();
	void setValue(T value);
	T getValue();

};

template <class T> SharedMemory<T> :: SharedMemory () {
	this->shmId = 0;
	this->dataPointer = NULL;
}

template <class T> SharedMemory<T> :: ~SharedMemory () {
}

template <class T> int SharedMemory<T> :: create ( key_t key ) {
	// create shared memory
	this->shmId = shmget(key, sizeof(T), 0644|IPC_CREAT);

	if ( this->shmId == -1 )
		return SHMGET_ERROR;
	else {
		// attach memory block to process address space
		void* temp = shmat(this->shmId, NULL, 0);

		if ( temp == (void *) -1 ) {
			return SHMAT_ERROR;
		} else {
			this->dataPointer = (T *) temp;
			return SHM_OK;
		}
	}
}


template <class T> void SharedMemory<T> :: release () {
	// detach memory block
	shmdt ((void *) this->dataPointer );

	int attachedProcesses = this->attachedProcessesCount();

	if (attachedProcesses == 0) {
		shmctl ( this->shmId,IPC_RMID,NULL );
	}
}

template <class T> void SharedMemory<T> :: setValue ( T value ) {
	*(this->dataPointer) = value;
}

template <class T> T SharedMemory<T> :: getValue () {
	return (*(this->dataPointer));
}

template <class T> int SharedMemory<T> :: attachedProcessesCount () {
	shmid_ds state;
	shmctl (this->shmId, IPC_STAT, &state);
	return state.shm_nattch;
}

#endif /* SHAREDMEMORY_H_ */

