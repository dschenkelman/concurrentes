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

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 2){
		std::cout << "Usage is ConcuPig <numberOfPlayers>" << endl;
		std::cin.get();
		return 0;
	}

	// int players = Convert::ToInt(argv[1]);
	int players = 3;

	cout << "Hello world " << players << endl; // prints Hello world

	ConcurrencyManager manager(players);
	manager.initialize();

	int startedPlayers = 0;
	vector<pid_t> playerProcesses;

	while (startedPlayers != players){
		pid_t childProcess = fork();

		if (childProcess == 0){
			PlayerHead player(startedPlayers, startedPlayers == 0 ? players - 1 : startedPlayers - 1, startedPlayers + 1 % players);
			player.run();
			break;
		}
		else{
			playerProcesses.push_back(childProcess);
			startedPlayers++;
		}
	}

	if (fork() == 0){
		Table table(players, playerProcesses);
		table.run();
	}
	else if (fork() == 0){
		PlayerSynchronizer sync(players);
		sync.run();
	}
	else if (fork() == 0){
		ScoreBoardController controller;
		controller.run();
	}

	while (wait(NULL) > 0){
		// improve the way this waits for all children to be done
	}

	manager.terminate();

	return 0;
}
