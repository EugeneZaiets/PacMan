#pragma once
#ifndef PACMAN_H
#define PACMAN_H

#include "Game.h"
#include "Actor.h"
#include "ConsoleSettingsHandler.h"

#include <iostream>
#include <conio.h>
#include <ctime>
#include <Windows.h>
using namespace Game_Constants;

class Game;

class PacMan : public iActor
{
private:
	int x, y;
	int prev_x, prev_y;

	bool got_energizer;
    unsigned int lives;
	unsigned int score;
	unsigned int color;
	unsigned int speed;
	unsigned int kill_counter;
	unsigned int move_counter;
	unsigned int score_offset;
	unsigned char direction;
	unsigned char old_direction;
	unsigned char head;
	std::clock_t timer;

	std::shared_ptr<ConsoleSettingsHandler> m_console_handler;
	Game* game_instance;
public:
	PacMan(std::shared_ptr<ConsoleSettingsHandler> console_handler, Game* game);
	~PacMan();

	void Move();
	void Dead();
	void ScoreUp();
	void RenderScore();
	void RenderLives();
	char GetDirection();
	void RenderPacman();
	void RenderKill();
	void ResetPacMan(int x, int y);
	bool CheckCollision (char dir);


	int GetPos_X()                   { return x;             }
	int GetPos_Y()                   { return y;             }
	int GetLives()                   { return lives;         }
	void SetDirection(char dir)      { direction = dir;      }
	void SetOldDirection(char dir)   { old_direction = dir;  }
	void SetPos_X(int x_pos)         { x = x_pos;            }
	void SetPos_Y(int y_pos)         { y = y_pos;            }
	void SetLives(int num)           { lives = num;          }
	void SetEnergizerInfo(bool info) { got_energizer = info; }

	bool GetEnergizerInfo()          { return got_energizer; }
	double GetTimeInBooste()         { return (std::clock() - timer) / (double)CLOCKS_PER_SEC; }
};
#endif //PACMAN_H