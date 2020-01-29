#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <map>
#include <string>

namespace game_constants
{
    const int X_SIZE = 46; //Maze x_size
    const int Y_SIZE = 21; //Maze y_size

    const int X_GAME_SCREEN_OFFSET             = 0;
    const int Y_GAME_SCREEN_BOTTOM_SIDE_OFFSET = 2;
    const int Y_GAME_SCREEN_TOP_SIDE_OFFSET    = 1;

    const int X_GAME_SCREEN_SIZE = X_SIZE;                           //Screen x_size
    const int Y_GAME_SCREEN_SIZE = Y_SIZE + 
                                   Y_GAME_SCREEN_TOP_SIDE_OFFSET + 
                                   Y_GAME_SCREEN_BOTTOM_SIDE_OFFSET; //Screen y_size

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

    //In game constants
    const int OFFSET_PINKY_POSITION = 4;
    const int POINTS_NUMBER         = 360;
    const int NUMBER_OF_GHOSTS      = 4;
    const int NUMBER_OF_LIVES       = 3;
    const int PACMAN_SPEED          = 28;
    const int GHOST_SPEED           = 58;

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
        HEAD_RIGHT = '<',
        HEAD_LEFT  = '>',
        HEAD_DOWN  = '^',
        HEAD_UP    = 'v'
    };
    enum Mode 
    { 
        MODE_CHASE,
        MODE_SCATTER,
        MODE_FRIGHTENED,
        MODE_DEAD,
        MODE_WAIT,
        MODE_EXIT_GATE,
        MODE_ENTER_GATE
    };
    enum Ghost_Name
    { 
        GHOST_NAME_BLINCKY, 
        GHOST_NAME_PINKY,
        GHOST_NAME_INKY,
        GHOST_NAME_CLYDE
    };
    const char GHOST_HEAD          = 'G';
    const char DEFAULT_DIRECTION   = 'w';
    const char NO_DIRECTION        = 'N';
    const char CharNotToCollide[3] = { ' ', 'o', static_cast<char>(250) };
    const char Direction[4]        = { 'w', 'a', 's', 'd' };

    const std::map<char, short> MapCharToPtint = 
    {
        { ' ', 32  },
        { '.', 250 },
        { 'o', 111 },
        { '1', 201 },
        { '2', 187 },
        { '3', 200 },
        { '4', 188 },
        { '5', 205 }, 
        { '6', 186 }, 
        { '^', 217 }, 
        { '$', 218 },
        { '%', 179 }, 
        { '#', 196 }, 
        { '@', 192 }, 
        { '*', 191 }
    };
   
    //Menu Constants
    const int NUM_OF_MENU_TITLES = 4;
    const int UNDEFINED_CHOISE   = -1;
    const std::string menu_titles[NUM_OF_MENU_TITLES] =
    {
        "NEW GAME",
        "title 1",
        "title 2",
        "EXIT"
    };
    const char MENU_CURSOR = '>';

    //Virtual Keys
    const int VK_W = 0x57;
    const int VK_A = 0x41;
    const int VK_S = 0x53;
    const int VK_D = 0x44;

    //Timings
    const double SECODS_IN_BOOST_BY_LEVEL = 20.0;
    const double SECODS_TO_CHANGE_MODE    = 20.0;
}
#endif //CONSTANTS_H