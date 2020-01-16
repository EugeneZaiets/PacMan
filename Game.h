#pragma once
#ifndef GAME_H
#define GAME_H

#include "ConsoleSettingsHandler.h"
#include "constants.h"
#include "PacMan.h"
#include "Ghost.h"

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <memory>
#include <ctime>
#include <vector>
using namespace std;
using namespace Game_Constants;

class PacMan;
class Ghost;
class ConsoleSettingsHandler;

class Game
{
	int points_num;
	int level_counter;
	int number_of_ghosts;
	char m_MapToPrint[Y_SIZE][X_SIZE];
	double seconds_in_boost_by_level;
	double seconds_to_mode_change;

	std::shared_ptr<ConsoleSettingsHandler> m_console_handler;
	std::unique_ptr<PacMan> pacman;
	std::unique_ptr<Ghost> ghost[4];

	std::clock_t timer;
	std::clock_t timer2;
	void game_Loop();
	void loadLevel();
	void render();
	bool isDead();
	void moveGhosts();
	void handleTime();
	void initAllActors();
	void setMazeText(std::string, int);
	void resetMapInCollision();
	bool collisionWithGhost();
    void checkPointersToActors();
    void determitePositionForModeActivity();
public:
	Game(std::shared_ptr<ConsoleSettingsHandler> console_handler);
	~Game();
	void start();

	void getCharFromMap(char, int, int);
	void decreasePointsNum()                    { --points_num;                  }
	int  getPointsNum()                         { return points_num;             }
	char getCharOfMap(int x, int y)             { return m_MapToPrint[y][x];     }
	void setCharOfMap(int x, int y, char print) { m_MapToPrint[y][x] = print;    }
	void setPointsNum(int num)                  { points_num = num;              }

	double getTime()  { return (std::clock() - timer) / (double)CLOCKS_PER_SEC;  }
	double getTime2() { return (std::clock() - timer2) / (double)CLOCKS_PER_SEC; }
};
#endif // GAME_H