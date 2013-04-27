//============================================================================
// Name        : ConcuPig.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
#include "Helpers/Convert.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 2){
		std::cout << "Usage is ConcuPig <numberOfPlayers>" << endl;
		std::cin.get();
		return 0;
	}

	int players = Convert::ToInt(argv[1]);

	cout << "Hello world " << players << endl; // prints Hello world
	return 0;
}
