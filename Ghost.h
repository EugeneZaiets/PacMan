#pragma once
#ifndef GHOST_H
#define GHOST_H

#include "Game.h"
#include "Actor.h"
#include "constants.h"
#include "ConsoleSettingsHandler.h"
#include "CareTakerGhost.h"

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <utility>
#include <ctime>
using namespace game_constants;

class Game;
class CareTakerGhost;
class Memento;

class Ghost : public iActor
{
public:
    Ghost(std::shared_ptr<ConsoleSettingsHandler> console_handler, Game* game, Ghost_Name his_name);
    ~Ghost();

    void       dead();
    void       renderMap();
    void       renderGhost();
    void       resetModes         (const int name);
    void       resetGhost         (const int x, const int y);
    void       modeActivity       (const int x, const int y, const bool paused);

    void       move               (const char);
    const bool isPaused           (const bool);
    const bool checkCollision     (const char);

    const int  getClydeCountPos_X (const int pacman_x);
    const int  getClydeCountPos_Y (const int pacman_y);
    const int  getInkyPos_X       (const int pacman_x, const int blincky_x);
    const int  getInkyPos_Y       (const int pacman_y, const int blincky_y);


    const int        getPos_X();
    const int        getPos_Y();
    const int        getColor();
    const Mode       getMode ();
    const Ghost_Name getName ();
    const char       getDirection();
    const double     getTimeInWait();

    void setColor        (const Ghost_Name ghost_name);
    void setMode         (const Mode mode);
    void setHead         (const char head);
    void setPrevMode     (const Mode mode);
    void setColor        (const int color);
    void setPos_X        (const int x_pos);
    void setPos_Y        (const int y_pos);
    void setDirection    (const char direction);
    void setOldDirection (const char direction);
    /*std::unique_ptr<Memento> createMemento() { return std::make_unique<Memento>(*this); }
    void restoreToMemento(std::unique_ptr<Memento> memento) { *this = memento->getOriginator(); }*/

private:

    int           m_x_, m_y_           ;
    int           m_prev_x_, m_prev_y_ ;
    unsigned int  m_color_             ;
    unsigned int  m_speed_             ;
    unsigned int  m_move_counter_      ;
    unsigned char m_direction_         ;
    unsigned char m_old_direction_     ;
    unsigned char m_head_              ;
    bool          m_check_to_unpause_  ;
    Mode          m_current_mode_      ;
    Mode          m_prev_mode_         ;
    std::clock_t  m_timer_             ;
    std::clock_t  m_timer_on_pause_    ;
    Ghost_Name    m_name_              ;
    Game*         m_game_instance_     ;

    std::shared_ptr<ConsoleSettingsHandler> m_console_handler_ ;
    //std::unique_ptr<CareTakerGhost> caretaker_ghost;

    void handleScatterMode();
    void handleExitMode();
    void handleDeadMode();
    void handleWaitMode();
    void handleChaseMode      (const int x, const int y);
    void handleFrightenedMode (const int x, const int y);

    const char getOppositeDirection();
    const char determineClosestMove  (const int pm_x, const int pm_y);
    const char determineFurthestMove (const int pm_x, const int pm_y);
    const int  offsetCoordinatesX    (const int);
    const int  offsetCoordinatesY    (const int);
};
#endif //GHOST_H