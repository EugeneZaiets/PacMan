#pragma once
#ifndef GHOST_H
#define GHOST_H

#include "Game.h"
#include "Actor.h"
#include "constants.h"
#include "ConsoleSettingsHandler.h"

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <utility>
#include <ctime>
using namespace Game_Constants;

class Game;

class Ghost : public iActor
{
	int x, y;
	int prev_x, prev_y;
	unsigned int color;
	unsigned int speed;
	unsigned int move_counter;
	unsigned char direction;
	unsigned char old_direction;
	unsigned char head;
    bool check_to_unpause;

	Mode current_mode;
	Mode prev_mode;
	Ghosts_Names name;
	std::shared_ptr<ConsoleSettingsHandler> m_console_handler;
	Game* game_instance;
	std::clock_t timer;
    std::clock_t timer_on_pause;

    void handleChaseMode(int x, int y);
	void handleScatterMode();
    void handleFrightenedMode(int x, int y);
    void handleExitMode();
    void handleDeadMode();
    void handleWaitMode();
	char determineClosestMove(int pm_x, int pm_y);
	char determineFurthestMove(int pm_x, int pm_y);
	char getOppositeDirection();
	int  offsetCoordinatesX(int);
	int  offsetCoordinatesY(int);
public:
	Ghost(std::shared_ptr<ConsoleSettingsHandler> console_handler, Game* game, Ghosts_Names his_name);
	~Ghost();

	void move(char);
	void dead();
	bool checkCollision(char);
    bool isPaused(bool);
	void renderMap();
	void renderGhost();
	void resetGhost(int x, int y);
	void resetModes(int name);
	void modeActivity(int x, int y, bool paused);

	int getInkyPos_X(int pacman_x, int blincky_x);
	int getInkyPos_Y(int pacman_y, int blincky_y);
	int getClydeCountPos_X(int pacman_x);
	int getClydeCountPos_Y(int pacman_y);
	double getTimeInWait() { return (std::clock() - timer) / (double)CLOCKS_PER_SEC; }

	int getPos_X()                 { return x;            }
	int getPos_Y()                 { return y;            }
	int getGhostColor()            { return color;        }
	Ghosts_Names getGhostName()    { return name;         }
	Mode getMode()                 { return current_mode; }
	void setMode(Mode m)           { current_mode = m;    }
	void setHead(char ch)          { head = ch;           }
	void setPrevMode(Mode m)       { prev_mode = m;       }
	void setPos_X(int x_pos)       { x = x_pos;           }
	void setPos_Y(int y_pos)       { y = y_pos;           }
	void setDirection(char dir)    { direction = dir;     }
	void setOldDirection(char dir) { old_direction = dir; }
	void setGhostColor(int clr)    { color = clr;         }
    char getDirection()            { return 0;            }

	void setGhostColor(Ghosts_Names name);
};
#endif //GHOST_H