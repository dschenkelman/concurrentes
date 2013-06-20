#include "Convert.h"
#include <string>
#include <sstream>

using namespace std;

string Convert::toString(int number){
	ostringstream convert;
	convert << number;
	return convert.str();
}

int Convert::toInt(char* value){
	string str(value);

	int number;
	istringstream(str) >> number;

	return number;
}
