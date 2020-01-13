#pragma once
#include <chrono>

const int X_SIZE = 46; //Buffer x_size
const int Y_SIZE = 21; //Buffer y_size

const int X_SCREEN_SIZE = X_SIZE; //Screen x_size
const int Y_SCREEN_SIZE = Y_SIZE + 3; //Screen x_size

const int X_GATE = 19; 
const int Y_GATE = 10; 

//ERRORS
const int WINDOW_INFO_FAIL		  = 2;
const int CONSOLE_BUFFERSIZE_FAIL = 3;

//COLORS
const int RED        = 4;
const int BLUE       = 1;
const int PINK	     = 5;
const int CYAN		 = 3;
const int WHITE	 	 = 15;
const int BLACK	 	 = 0;
const int YELLOW     = 6;
const int ORANGE	 = 13;
const int LIGHT_BLUE = 9;


//Elements declaration
enum Head { RIGHT = '<', LEFT = '>', DOWN =  '^', UP = 'v' };
enum Mode {CHASE, SCATTER, FRIGHTENED, DEAD, WAIT, EXIT_GATE, ENTER_GATE};
enum Ghosts_Names {BLINCKY, PINKY, INKY, CLYDE};

const char CharNotToCollide[3] = {' ', 'o', 250 /*pill ascii*/ };
const char Direction[4] = { 'w', 'a', 's', 'd' };
