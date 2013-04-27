//============================================================================
// Name        : ConcuPig.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Services/Logger.h"
#include <stdlib.h>
#include "Helpers/Convert.h"
#include "Services/ConcurrencyManager.h"
#include <sys/fcntl.h>

#include "Model/SharedCard.h"

using namespace std;

int main(int argc, char* argv[]) {
	/*if (argc != 2){
		std::cout << "Usage is ConcuPig <numberOfPlayers>" << endl;
		std::cin.get();
		return 0;
	}

	int players = Convert::ToInt(argv[1]);

	cout << "Hello world " << players << endl; // prints Hello world*/

	ConcurrencyManager manager(5);
	manager.initialize();

	Logger *sc1;
	sc1 = Logger::getInstance();
	string s1 = "Logging something";
	sc1->logLine(s1,INFO);
	cout << "Goodbye world" << endl;

	manager.terminate();

	return 0;
}
