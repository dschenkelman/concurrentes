#include "Lock.h"

Lock :: Lock ( std::string& name ) {

	this->name = name;

	this->fl.l_type = F_WRLCK;
	this->fl.l_whence = SEEK_SET;
	this->fl.l_start = 0;
	this->fl.l_len = 0;
	this->fl.l_pid = getpid ();
	this->fd = open ( this->name.c_str(),O_CREAT|O_WRONLY,0777 );

	acquire();
}

Lock :: ~Lock () {
	close ( this->fd );
	release();
}

//Private methods

int Lock :: acquire () {
	this->fl.l_type = F_WRLCK;
	int result = fcntl ( this->fd,F_SETLKW,&(this->fl) );
	return result;
}

int Lock :: release () {
	this->fl.l_type = F_UNLCK;
	int result = fcntl ( this->fd,F_SETLK,&(this->fl) );
	return result;
}

