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

	void move();
	void dead();
	void scoreUp();
	void renderScore();
	void renderLives();
	char getDirection();
	void renderPacman();
	void renderKill();
	void resetPacMan(int x, int y);
	bool checkCollision (char dir);


	int getPos_X()                   { return x;             }
	int getPos_Y()                   { return y;             }
	int getLives()                   { return lives;         }
	void setDirection(char dir)      { direction = dir;      }
	void setOldDirection(char dir)   { old_direction = dir;  }
	void setPos_X(int x_pos)         { x = x_pos;            }
	void setPos_Y(int y_pos)         { y = y_pos;            }
	void setLives(int num)           { lives = num;          }
	void setEnergizerInfo(bool info) { got_energizer = info; }

	bool getEnergizerInfo()          { return got_energizer; }
	double getTimeInBooste()         { return (std::clock() - timer) / (double)CLOCKS_PER_SEC; }
};
#endif //PACMAN_H