/*
 * PersonRegister.h
 *
 *  Created on: 20/06/2013
 *      Author: matias
 */

#ifndef PERSONREGISTER_H_
#define PERSONREGISTER_H_

#define SIZE_NAME 61
#define SIZE_ADDRESS 120
#define SIZE_TELEPHONE 13

typedef struct person
{
	char name[SIZE_NAME];
	char address[SIZE_ADDRESS];
	char telephone[SIZE_TELEPHONE];
} person;

#endif /* PERSONREGISTER_H_ */
