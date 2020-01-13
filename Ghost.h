#pragma once
#ifndef GHOST_H
#define GHOST_H

#include "Game.h"
#include "Actor.h"
#include "constants.h"
#include "SetConsole.h"

#include <conio.h>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <utility>

class Game;

class Ghost : public Actor
{
private:
	int x, y;
	int prev_x, prev_y;
	int init_pos_x, init_pos_y;
	unsigned int color;
	unsigned int speed;
	unsigned int move_counter;
	unsigned char direction;
	unsigned char old_direction;
	unsigned char head;

	Mode current_mode;
	Mode prev_mode;
	Ghosts_Names name;
	ConsoleSettingsHandler* m_console_handler;
	Game* game_instance;
	std::clock_t timer;


	void HandleScatterMode();
	char DetermineClosestMove(int pm_x, int pm_y);
	char DetermineFurthestMove(int pm_x, int pm_y);
	char GetOppositeDirection();
	int OffsetCoordinatesX(int);
	int OffsetCoordinatesY(int);
public:
	Ghost(ConsoleSettingsHandler* console_handler, Game* game, Ghosts_Names his_name);
	~Ghost();

	void Move(char);
	void Dead();
	bool CheckCollision(char);
	void RenderMap();
	void RenderGhost();
	void ResetGhost(int x, int y);
	void ResetModes(int name);
	void ModeActivity(int x, int y);
	char GetDirection();

	int GetInkyPos_X(int pacman_x, int blincky_x);
	int GetInkyPos_Y(int pacman_y, int blincky_y);
	int GetClydeCountPos_X(int pacman_x);
	int GetClydeCountPos_Y(int pacman_y);
	double GetTimeInWait() { return (std::clock() - timer) / (double)CLOCKS_PER_SEC; }

	int GetPos_X()                 { return x;            }
	int GetPos_Y()                 { return y;            }
	int GetGhostColor()            { return color;        }
	Ghosts_Names GetGhostName()    { return name;         }
	Mode GetMode()                 { return current_mode; }
	void SetMode(Mode m)           { current_mode = m;    }
	void SetHead(char ch)          { head = ch;           }
	void SetPrevMode(Mode m)       { prev_mode = m;       }
	void SetPos_X(int x)           { this->x = x;         }
	void SetPos_Y(int y)           { this->y = y;         }
	void SetDirection(char dir)    { direction = dir;     }
	void SetOldDirection(char dir) { old_direction = dir; }
	void SetGhostColor(int clr)    { color = clr;         }

	void SetGhostColor(Ghosts_Names name);
};
#endif //GHOST_H