#include "ScoreBoardController.h"
#include "../Concurrency/Lock.h"
#include "../Services/Logger.h"

#include <iostream>
#include <stdio.h>
#include <string>

#define ESC_CODE 27

using namespace std;

ScoreBoardController::ScoreBoardController(int players): scoreBoardMemory(players, false){ }

ScoreBoardController::~ScoreBoardController() {
	string message = "Destroying scoreboard controller";
	Logger::getInstance()-> logLine(message, INFO);
}

void ScoreBoardController::run()
{
	char readKey;
	bool reloop = true;
	std::string message = "Press q to exit, another key to see the ScoreBoard";

	do
	{
		cout << message << endl;
		readKey = getchar();
		if('q' == readKey )
		{
			reloop = false;
		}else
		{
			this->scoreBoardMemory.print();
		}
	}while( reloop );

	// Final scoreboard
	this->scoreBoardMemory.print();
}

