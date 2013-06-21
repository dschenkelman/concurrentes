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

//Used for messages responses
#define HEAD 5
#define OPERATION_FAILED 6
#define ENDOFCONNECTION 7
#define OPERATION_CREATE_SUCCESS 8
#define OPERATION_UPDATE_SUCCESS 9
#define OPERATION_DELETE_SUCCESS 10
#define OPERATION_UNKNOWN 11
#define BODY 12

//Used for end of connection
#define GRACEFUL_QUIT 13
#define NULL_ACTION_TYPE 14



#endif /* MESSAGEACTIONS_H_ */
