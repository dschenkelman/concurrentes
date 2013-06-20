/*
 * DataBaseManager.h
 *
 *  Created on: 20/06/2013
 *      Author: matias
 */

#ifndef DATABASEMANAGER_H_
#define DATABASEMANAGER_H_

#include <string.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <list>
#include <stdio.h>

#include "PersonRegister.h"

class DataBaseManager
{
public:
	static DataBaseManager *getInstance();
	virtual ~DataBaseManager();

	bool initialize();

	std::list<struct person> retrievePersons(
			std::string namePattern,
			std::string addressPattern,
			std::string phonePattern);

	bool createPerson(struct person person);
	bool updatePerson(char *nameId, struct person person, bool createIfNeeded);
	bool deletePerson(struct person person);

	bool finalize();

protected:
	DataBaseManager();

private:
	std::list<struct person> persons;
	static DataBaseManager *instance;
	static const std::string dbFile;
};

#endif /* DATABASEMANAGER_H_ */
