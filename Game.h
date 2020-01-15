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

	ConsoleSettingsHandler* m_console_handler;
	PacMan* pacman;
	Ghost* ghost[4];

	std::clock_t timer;
	void Game_Loop();
	void LoadLevel();
	void Render();
	bool isDead();
	void MoveGhosts();
	void HandleTime();
	void InitAllActors();
	void SetMazeText(std::string, int);
	void ResetMapInCollision();
	bool CollisionWithGhost();
public:
	Game(ConsoleSettingsHandler* console_handler);
	~Game();
	void Start();

	void DecreasePointsNum()                    { --points_num;               }
	int  GetPointsNum()                         { return points_num;          }
	char GetCharOfMap(int x, int y)             { return m_MapToPrint[y][x];  }
	void SetCharOfMap(int x, int y, char print) { m_MapToPrint[y][x] = print; }
	void SetPointsNum(int num)                  { points_num = num;           }

	double GetTime(){ return (std::clock() - timer) / (double)CLOCKS_PER_SEC; }
};

#endif // GAME_H