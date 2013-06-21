#include "DataBaseManager.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

DataBaseManager* DataBaseManager::instance = NULL;

const string DataBaseManager::dbFile = "DBFile";

DataBaseManager::DataBaseManager()
{
}

DataBaseManager::~DataBaseManager()
{
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

	char name[61],address[120],telephone[13];

    FILE *fd = fopen(this->dbFile.c_str(), "r");

	if (fd == NULL){
		perror("Database couldn't be found or read");
		return false;
	}

    while (fscanf(fd, "%61[^;];%120[^;];%s\n",name,address,telephone) == 3){
    	person pAux;
    	strcpy(pAux.name,name);
    	strcpy(pAux.address,address);
    	strcpy(pAux.telephone,telephone);

    	this->persons.push_back(pAux);
    }

    fclose(fd);

    /*for (list<struct person>::iterator it = persons.begin(); it != persons.end(); it++){
    	cout << it->name << ";" << it->address << ";" << it->telephone << endl;
    }*/

	return true;
}


std::list<struct person> DataBaseManager::retrievePersons(
		std::string namePattern,
		std::string addressPattern,
		std::string phonePattern)
{
	//cout << "Retrieving : " << namePattern << " | " << addressPattern << " | " << phonePattern << endl;
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
	std::list<struct person>::iterator iterator = persons.begin();

	while( persons.end() != iterator )
	{
		if( filterByName )
		{
			std::string name(iterator->name);
			if( string::npos == name.find(namePattern) )
			{
				iterator++;
				continue;
			}
		}
		if( filterByAddress )
		{
			std::string address(iterator->address);
			if( string::npos == address.find(addressPattern) )
			{
				iterator++;
				continue;
			}
		}
		if( filterByPhone )
		{
			std::string phone(iterator->telephone);
			if( string::npos == phone.find(phonePattern) )
			{
				iterator++;
				continue;
			}
		}
		filteredPersons.push_back(*iterator);
		iterator++;
	}
	return filteredPersons;
}

bool DataBaseManager::createPerson(struct person person)
{
	persons.push_back(person);
	return true;
}

bool DataBaseManager::updatePerson(char *nameId, struct person person, bool createIfNeeded)
{
	//cout << "Updating : " << person.name << " | " << person.address << " | " << person.telephone << endl;
	//cout << "With person Name Id : " << nameId << endl;

	bool notFound = true;

	std::list<struct person>::iterator personIterator =
			persons.begin();
	while(notFound && (personIterator != persons.end()))
	{
		struct person iteratedPerson = *personIterator;

		//cout << person.name << " == " << nameId << " -> ";
		if( 0 == strcmp(personIterator->name, nameId) )
		{
			notFound = false;
			strcpy(personIterator->name, person.name);
			strcpy(personIterator->address, person.address);
			strcpy(personIterator->telephone, person.telephone);
			//cout << "true" << endl;
			return true;
		}
		else
		{
			//cout << "false" << endl;
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
	FILE * fd;
	fd = fopen (this->dbFile.c_str(),"w+");

	if (fd == NULL){
		perror("Database couldn't be found or read");
		return false;
	}

	for (list<struct person>::iterator it = persons.begin(); it != persons.end(); it++){
		fprintf (fd, "%s;%s;%s\n", it->name, it->address, it->telephone);
	}

	fclose (fd);

	return true;
}