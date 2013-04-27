/*
 * Convert.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: dschenkelman
 */

#include "Convert.h"
#include <string>
#include <sstream>

using namespace std;

string Convert::ToString(int number){
	ostringstream convert;
	convert << number;
	return convert.str();
}

int Convert::ToInt(char* value){
	string str(value);

	int number;
	istringstream(str) >> number;

	return number;
}

