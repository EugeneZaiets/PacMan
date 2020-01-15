#pragma once
namespace Game_Constants
{
	const int X_SIZE = 46; //Maze x_size
	const int Y_SIZE = 21; //Maze y_size

	const int X_GAME_SCREEN_OFFSET = 0;
	const int Y_GAME_SCREEN_OFFSET = 3;

	const int X_GAME_SCREEN_SIZE = X_SIZE;                        //Screen x_size
	const int Y_GAME_SCREEN_SIZE = Y_SIZE + Y_GAME_SCREEN_OFFSET; //Screen x_size

	const int X_GATE = 19;
	const int Y_GATE = 10;

	const int X_MIDDLE_POS = 19;
	const int Y_MIDDLE_POS = 7;

	//ERRORS
	const int WINDOW_INFO_FAIL        = 2;
	const int CONSOLE_BUFFERSIZE_FAIL = 3;

	//COLORS
	const int RED        = 4;
	const int BLUE       = 1;
	const int PINK       = 5;
	const int CYAN       = 3;
	const int WHITE      = 15;
	const int BLACK      = 0;
	const int YELLOW     = 6;
	const int ORANGE     = 13;
	const int LIGHT_BLUE = 9;

	
	const int OFFSET_PINKY_POSITION = 4;
	const int POINTS_NUMBER         = 360;

	//Position for ghosts in scatter mode
	const int BLINKY_SCATTER_POS_X  = 44;
	const int BLINKY_SCATTER_POS_Y  = 1;
	const int PINKY_SCATTER_POS_X   = 44;
	const int PINKY_SCATTER_POS_Y   = 20;
	const int INKY_SCATTER_POS_X    = 1;
	const int INKY_SCATTER_POS_Y    = 1;
	const int CLYDE_SCATTER_POS_X   = 4;
	const int CLYDE_SCATTER_POS_Y   = 20;

	//Init positions for actors
	const int PACMAN_INIT_POS_X = 35;
	const int PACMAN_INIT_POS_Y = 10;
	const int BLINKY_INIT_POS_X = 18;
	const int BLINKY_INIT_POS_Y = 10;
	const int PINKY_INIT_POS_X  = 20;
	const int PINKY_INIT_POS_Y  = 10;
	const int INKY_INIT_POS_X   = 21;
	const int INKY_INIT_POS_Y   = 11;
	const int CLYDE_INIT_POS_X  = 21;
	const int CLYDE_INIT_POS_Y  = 9;


	//Elements declaration
	enum Head 
	{ 
		RIGHT = '<', 
		LEFT = '>', 
		DOWN = '^', 
		UP = 'v' 
	};
	enum Mode 
	{ 
		CHASE,
		SCATTER, 
		FRIGHTENED, 
		DEAD, 
		WAIT, 
		EXIT_GATE, 
		ENTER_GATE 
	};
	enum Ghosts_Names 
	{ 
		BLINCKY, 
		PINKY, 
		INKY, 
		CLYDE 
	};

	const char GHOST_HEAD = 'G';
	const char CharNotToCollide[3] = { ' ', 'o', 250 /*pill ascii*/ };
	const char Direction[4] = { 'w', 'a', 's', 'd' };
}