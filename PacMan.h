#pragma once
#ifndef PACMAN_H
#define PACMAN_H

#include "Game.h"
#include "Actor.h"
#include "SetConsole.h"

#include <iostream>
#include <conio.h>
#include <ctime>
#include <Windows.h>

class Game;

class PacMan : public Actor
{
private:
	int x, y;
	int prev_x, prev_y;
	int init_pos_x, init_pos_y;
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

	ConsoleSettingsHandler* m_console_handler;
	Game* game_instance;
public:
	PacMan(ConsoleSettingsHandler* console_handler, Game* game);
	~PacMan();

	void Move();
	void Dead();
	void ScoreUp();
	void RenderScore();
	void RenderLives();
	void RenderPacman();
	void RenderKill();
	void ResetPacMan(int x, int y);
	bool CheckCollision (char dir);
	
	char GetDirection();
	int	 GetPos_X() { return x; }
	int  GetPos_Y() { return y; }
	int  GetLives() { return lives; }
	bool GetEnergizerInfo() { return got_energizer; }
	double GetTimeInBooste() { return (std::clock() - timer) / (double)CLOCKS_PER_SEC; }

	void SetDirection(char dir) { direction = dir; }
	void SetOldDirection(char dir) { old_direction = dir; }
	void SetPos_X(int x) { this->x = x; }
	void SetPos_Y(int y) { this->y = y; }
	void SetLives(int num) { lives = num; }
	void SetEnergizerInfo(bool info) { got_energizer = info; }
};
#endif //PACMAN_H