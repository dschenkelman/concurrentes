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

int DataBaseManager::createPerson(struct person person)
{
	if ( this->personExists(person.name) ){
		//Can't create a person with a name that already exists.
		return OPERATION_CREATE_FAIL_PERSON_EXISTS;
	}
	persons.push_back(person);
	return OPERATION_CREATE_SUCCESS;
}

int DataBaseManager::updatePerson(char *nameId, struct person person, bool createIfNeeded)
{
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

			if ( this->personExists(person.name) ){
				//Can't update name to a name that already exists.
				return OPERATION_UPDATE_FAIL_PERSON_EXISTS;
			}
			strcpy(personIterator->name, person.name);
			strcpy(personIterator->address, person.address);
			strcpy(personIterator->telephone, person.telephone);
			//cout << "true" << endl;
			return OPERATION_UPDATE_SUCCESS;
		}
		else
		{
			personIterator++;
		}
	}

	return OPERATION_UPDATE_FAIL_PERSON_NOT_EXISTS;
}

int DataBaseManager::deletePerson(struct person person)
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

	if ( notFound ){
		return OPERATION_DELETE_FAIL_PERSON_NOT_EXISTS;
	}

	persons.erase(personIterator);
	return OPERATION_DELETE_SUCCESS;

}

bool  DataBaseManager::personExists(char personName[SIZE_NAME])
{
	bool personExists = false;

	for (list<struct person>::iterator it = persons.begin(); it != persons.end(); it++){
		if( 0 == strcmp(it->name, personName) )
		{
			personExists = true;
			break;
		}
	}

	return personExists;
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
