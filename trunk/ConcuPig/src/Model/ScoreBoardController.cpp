#include "ScoreBoardController.h"
#include "SharedScoreboard.h"
#include "../Concurrency/Lock.h"

#include <iostream>
#include <stdio.h>
#include <string>

#define ESC_CODE 27

using namespace std;

ScoreBoardController::ScoreBoardController(int players): scoreBoardMemory(players){ }

ScoreBoardController::~ScoreBoardController() { }

void ScoreBoardController::run()
{
	char readKey;
	bool reloop = true;
	std::string message = "Press ESC to exit, another key to see the ScoreBoard";

	do
	{
		cout << message << endl;
		readKey = getchar();
		if(ESC_CODE == readKey )
		{
			reloop = false;
		}else
		{
			this->scoreBoardMemory.print();
		}
	}while( reloop );
}

