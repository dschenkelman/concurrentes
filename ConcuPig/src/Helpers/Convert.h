/*
 * Convert.h
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#ifndef CONVERT_H_
#define CONVERT_H_

#include <string>

class Convert {
public:
	static std::string toString(int number);
	static int toInt(char* value);
};

#endif /* CONVERT_H_ */
