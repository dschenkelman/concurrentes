#ifndef LOCK_H_
#define LOCK_H_

#include <unistd.h>
#include <fcntl.h>
#include <string>

class Lock {

private:

	struct flock fl;
	int fd;
	std::string name;

public:

	Lock ( std::string& name );
	virtual ~Lock();

private:
	int acquire ();
	int release ();
};

#endif /* LOCK_H_ */
