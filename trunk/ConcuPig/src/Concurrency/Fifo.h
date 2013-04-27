/*
 * Fifo.h
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#ifndef FIFO_H_
#define FIFO_H_

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>

class Fifo {

	private:
		int fileDescriptor;
		std::string name;
	public:
		Fifo (const std::string& name);
		virtual ~Fifo();

		int writeValue(char* value, int valueSize);
		int readValue(char* buffer, int bufferSize );
		void closeFifo();
		void eliminate();
};

#endif /* FIFO_H_ */
