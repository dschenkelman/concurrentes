#ifndef SEMPAHORE_H_
#define SEMPAHORE_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <string>

class Semaphore {

private:
	int id;
	int initialValue;

	int initialize ();

public:
	Semaphore (key_t key, int initialValue);
	virtual ~Semaphore();

	int wait ();
	int signal ();
	void deleteSemaphore ();
};


#endif /* SEMPAHORE_H_ */
