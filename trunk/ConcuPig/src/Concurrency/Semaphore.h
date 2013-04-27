#ifndef SEMPAHORE_H_
#define SEMPAHORE_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <string>

class Semaphore {

private:
	int id;

public:
	Semaphore (key_t key);
	virtual ~Semaphore();

	int initialize (int initialValue);
	int wait ();
	int signal ();
	void eliminate ();
};


#endif /* SEMPAHORE_H_ */
