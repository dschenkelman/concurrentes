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
#define OPERATION_CREATE_SUCCESS 7
#define OPERATION_UPDATE_SUCCESS 8
#define OPERATION_DELETE_SUCCESS 9
#define OPERATION_UNKNOWN 10
#define BODY 11



#endif /* MESSAGEACTIONS_H_ */
