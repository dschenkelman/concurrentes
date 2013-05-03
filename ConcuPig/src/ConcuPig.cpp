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
#include "Model/ScoreBoardController.h"
#include "Model/Table.h"
#include "Services/Logger.h"
#include "Helpers/Convert.h"
#include "Model/SharedCard.h"
#include "Player/PlayerHead.h"
#include "Player/PlayerCardReceiver.h"
#include "Player/PlayerCardSender.h"
#include <sys/wait.h>
#include <time.h>

using namespace std;

int main(int argc, char* argv[]) {
	srand( time( NULL ) ^ getpid() << 16 );
	if (argc != 2){
		std::cout << "Usage is ConcuPig <numberOfPlayers>" << endl;
		return 0;
	}

	int players = Convert::toInt(argv[1]);

	string line = "Start logging";
	Logger::getInstance()->logLine(line, INFO);

	cout << "Players " << players << endl;

	ConcurrencyManager manager(players);
	manager.initialize();

	Scoreboard scoreboard(players, true);

	int startedPlayers = 0;
	vector<pid_t> playerProcesses;

	string message;

	while (startedPlayers != players){
		pid_t childProcess = fork();

		if (childProcess == 0){

			pid_t receiverId = fork();

			if (receiverId == 0){
				PlayerCardReceiver receiver(startedPlayers, startedPlayers == 0 ? players - 1 : startedPlayers - 1);
				receiver.run();
				Logger::terminate();
				return 0;
			}
			else{
				message = "Receiver process for player " + Convert::toString(startedPlayers) + " is:" + Convert::toString(receiverId);
				Logger::getInstance()->logLine(message, INFO);

				pid_t senderId = fork();
				if (senderId == 0){
					PlayerCardSender sender(startedPlayers, (startedPlayers + 1) % players);
					sender.run();
					Logger::terminate();
					return 0;
				}
				else{
					message = "Sender process for player " + Convert::toString(startedPlayers) + " is:" + Convert::toString(senderId);
					Logger::getInstance()->logLine(message, INFO);

					srand( time( NULL ) ^ getpid() << 16 );
					PlayerHead player(startedPlayers, receiverId, senderId);
					message = "About to start running player " + Convert::toString(startedPlayers);
					Logger::getInstance()->logLine(message, INFO);
					player.run();
					Logger::terminate();
					return 0;
				}
			}
		}
		else{
			message = "Head process for player " + Convert::toString(startedPlayers) + " is:" + Convert::toString(childProcess);
			Logger::getInstance()->logLine(message, INFO);
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
		if (fork() == 0){
			Table table(players, syncId, playerProcesses);
			table.run();
		}
		else {
			ScoreBoardController controller(players);
			controller.run();

			scoreboard.print();

			manager.terminate();
			cout << "Game over" << endl;
		}
	}

	Logger::terminate();

	return 0;
}
