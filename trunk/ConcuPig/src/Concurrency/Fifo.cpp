/*
 * Fifo.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#include "Fifo.h"
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

using namespace std;

Fifo::Fifo (const string& name ) : fileDescriptor(-1), name(name) {
	// create FIFO
	mknod (this->name.c_str(), S_IFIFO|0666, 0);
}

Fifo :: ~Fifo () {
}

int Fifo::writeValue ( char* value,int valueSize ) {
	// open file descriptor for writing
	if ( this->fileDescriptor == -1) {
		this->fileDescriptor = open(this->name.c_str(), O_WRONLY);
	}

	// Write value to FIFO
	return write(this->fileDescriptor, (const void *)value, valueSize);
}

int Fifo :: readValue(char* buffer, int buffSize ) {
	// open file descriptor for reading
	if ( this->fileDescriptor == -1 ) {
		this->fileDescriptor = open(this->name.c_str(), O_RDONLY);
	}

	return read ( this->fileDescriptor, (void *)buffer, buffSize );
}

void Fifo :: closeFifo () {
	close(this->fileDescriptor);
}

void Fifo :: eliminate () {
	unlink(this->name.c_str());
}
