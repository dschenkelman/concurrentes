//============================================================================
// Name        : ConcuPig.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <vector>
#include "Services/ConcurrencyManager.h"
#include <sys/fcntl.h>
#include "Model/Scoreboard.h"
#include "Model/ScoreBoardController.h"
#include "Model/Table.h"
#include "Services/Logger.h"
#include "Helpers/Convert.h"
#include "Model/SharedCard.h"
#include "Player/PlayerHead.h"
#include <sys/wait.h>
#include <time.h>

using namespace std;

int main(int argc, char* argv[]) {
	srand( time( NULL ) );
//	if (argc != 2){
//		std::cout << "Usage is ConcuPig <numberOfPlayers>" << endl;
//		std::cin.get();
//		return 0;
//	}

	// int players = Convert::ToInt(argv[1]);

	int players = 4;

	string line = "Start logging";
	Logger::getInstance()->logLine(line, INFO);

	cout << "Players " << players << endl;

	ConcurrencyManager manager(players);
	manager.initialize();

	int startedPlayers = 0;
	vector<pid_t> playerProcesses;

	while (startedPlayers != players){
		pid_t childProcess = fork();

		if (childProcess == 0){
			PlayerHead player(startedPlayers, startedPlayers == 0 ? players - 1 : startedPlayers - 1, (startedPlayers + 1) % players);
			player.run();
			break;
		}
		else{
			playerProcesses.push_back(childProcess);
			startedPlayers++;
		}
	}

	pid_t syncId = fork();

	if (syncId == 0){
		PlayerSynchronizer sync(players);
		sync.run();
	}
	else {
		pid_t scoreboardId = fork();
		if (scoreboardId == 0){
			ScoreBoardController controller;
			controller.run();
		}
		else {
			Table table(players, syncId, scoreboardId, playerProcesses);
			table.run();

			int state = 0;

			for (int i = 0; i < (2 + players); i++){
				wait(&state);
			}

			manager.terminate();
			cout << "Game over" << endl;
		}
	}

	return 0;
}
