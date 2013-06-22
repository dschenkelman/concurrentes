/*
 * MessageActions.h
 *
 *  Created on: Jun 20, 2013
 *      Author: sebas
 */

#ifndef MESSAGEACTIONS_H_
#define MESSAGEACTIONS_H_

//Used for messages petitions
#define CREATE 1
#define UPDATE 2
#define READ 3
#define DELETE 4

//Used for end of connection
#define GRACEFUL_QUIT 5
#define NULL_ACTION_TYPE 6

//Used for messages responses
#define HEAD 7
#define BODY 8
#define OPERATION_FAILED 9
#define ENDOFCONNECTION 10
#define OPERATION_CREATE_SUCCESS 11
#define OPERATION_CREATE_FAIL_PERSON_EXISTS 12
#define OPERATION_UPDATE_SUCCESS 13
#define OPERATION_UPDATE_FAIL_PERSON_EXISTS 14
#define OPERATION_UPDATE_FAIL_PERSON_NOT_EXISTS 15
#define OPERATION_DELETE_SUCCESS 16
#define OPERATION_DELETE_FAIL_PERSON_NOT_EXISTS 17
#define OPERATION_UNKNOWN 18


#endif /* MESSAGEACTIONS_H_ */
