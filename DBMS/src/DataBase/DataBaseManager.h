/*
 * DataBaseManager.h
 *
 *  Created on: 20/06/2013
 *      Author: matias
 */

#ifndef DATABASEMANAGER_H_
#define DATABASEMANAGER_H_

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <list>

class DataBaseManager
{
public:
	static DataBaseManager *getInstance();
	virtual ~DataBaseManager();

	bool initialize();

protected:
	DataBaseManager();

private:
	std::list<std::string> registers;
	static DataBaseManager *instance;
};

#endif /* DATABASEMANAGER_H_ */
