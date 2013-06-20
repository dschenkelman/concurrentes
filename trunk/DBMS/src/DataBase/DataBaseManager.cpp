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
	// TODO mocked person registers
	struct person matias;
	strcpy(matias.name, "Matias Servetto");
	strcpy(matias.direction, "Ecuador 871");
	strcpy(matias.telephone, "1170707070");
	persons.push_back(matias);

	struct person sebastian;
	strcpy(sebastian.name, "Sebastian Rodriguez");
	strcpy(sebastian.direction, "Ugarte y Maipu");
	strcpy(sebastian.telephone, "1160606060");
	persons.push_back(sebastian);

	struct person damian;
	strcpy(damian.name, "Damian Schenkelman");
	strcpy(damian.direction, "Caballito");
	strcpy(damian.telephone, "1150505050");
	persons.push_back(damian);

	return true;
}


std::list<struct person> DataBaseManager::retrievePersons(
		std::string namePattern,
		std::string addressPattern,
		std::string phonePattern)
{
	bool filterByName 		= 0 != strcmp("*", namePattern.c_str());
	bool filterByAddress 	= 0 != strcmp("*", addressPattern.c_str());
	bool filterByPhone		= 0 != strcmp("*", phonePattern.c_str());
	if( 	false == filterByName 		&&
			false == filterByAddress 	&&
			false == filterByPhone )
	{
		return persons;
	}

	std::list<struct person> filteredPersons;



	return filteredPersons;
}

bool DataBaseManager::createPerson(struct person person)
{
	persons.push_back(person);
	return true;
}

bool DataBaseManager::updatePerson(struct person person, bool createIfNeeded)
{
	bool notFound = true;

	std::list<struct person>::iterator personIterator =
			persons.begin();
	while(notFound && (personIterator != persons.end()))
	{
		struct person iteratedPerson = *personIterator;

		if( 0 == strcmp(person.name, iteratedPerson.name) )
		{
			notFound = false;
			strcpy(iteratedPerson.name, person.name);
			strcpy(iteratedPerson.direction, person.direction);
			strcpy(iteratedPerson.telephone, person.telephone);
			return true;
		}
		else
		{
			personIterator++;
		}
	}
	if( notFound && createIfNeeded )
	{
		persons.push_back(person);
		return true;
	}
	return false;
}

bool DataBaseManager::deletePerson(struct person person)
{
	bool notFound = true;

	std::list<struct person>::iterator personIterator =
			persons.begin();
	while(notFound && (personIterator != persons.end()))
	{
		struct person iteratedPerson = *personIterator;

		if( 0 == strcmp(person.name, iteratedPerson.name) )
		{
			notFound = false;
		}else
		{
			personIterator++;
		}
	}
	if( false == notFound )
	{
		persons.erase(personIterator);
		return true;
	}
	return false;
}

bool DataBaseManager::finalize()
{

	return true;
}
