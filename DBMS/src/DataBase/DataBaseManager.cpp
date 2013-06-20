/*
 * DataBaseManager.cpp
 *
 *  Created on: 20/06/2013
 *      Author: matias
 */

#include "DataBaseManager.h"

DataBaseManager* DataBaseManager::instance = NULL;

DataBaseManager::DataBaseManager()
{
	// TODO Auto-generated constructor stub

}

DataBaseManager::~DataBaseManager()
{
	// TODO Auto-generated destructor stub
}

DataBaseManager *DataBaseManager::getInstance()
{
	if( NULL == DataBaseManager::instance )
	{
		DataBaseManager::instance = new DataBaseManager();
	}
	return DataBaseManager::instance;
}

bool DataBaseManager::initialize()
{
	return true;
}
