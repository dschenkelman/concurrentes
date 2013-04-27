/*
 * FileInitializer.h
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#ifndef FILEINITIALIZER_H_
#define FILEINITIALIZER_H_

class FileInitializer {
private:
	int players;
	void InitializeFifos(void);
	void InitializeSharedMemories(void);
	void InitializeSemaphores(void);
public:
	FileInitializer(int players);
	void Initialize(void);
	virtual ~FileInitializer();
};

#endif /* FILEINITIALIZER_H_ */
