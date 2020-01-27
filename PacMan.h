#pragma once
#ifndef PACMAN_H
#define PACMAN_H

#include "Game.h"
#include "Actor.h"
#include "ConsoleSettingsHandler.h"
#include "KeyboardInput.h"

#include <iostream>
#include <conio.h>
#include <ctime>
#include <Windows.h>
using namespace game_constants;

class Game;

class PacMan : public iActor, public Keyboard
{
public:
    PacMan(std::shared_ptr<ConsoleSettingsHandler> console_handler, Game* game);
    ~PacMan();

    void  dead();
    void  scoreUp();
    void  renderKill();
    void  renderScore();
    void  renderLives();
    void  renderPacman();
    const char getDirection();

    void       move           (const bool);
    const bool isPaused       (const bool);
    const bool checkCollision (const char);
    void       resetPacMan    (const int x, const int y);

    const int    getPos_X();
    const int    getPos_Y();
    const int    getLives();
    const bool   getEnergizerInfo();
    const double getTimeInBooste ();

    void setDirection     (const char direction);
    void setOldDirection  (const char direction);
    void setPos_X         (const int x_pos);
    void setPos_Y         (const int y_pos);
    void setLives         (const int lives);
    void setEnergizerInfo (const bool info);
    
private:

    int           m_x_, m_y_           ;
    int           m_prev_x_, m_prev_y_ ;
    unsigned int  m_lives_             ;
    unsigned int  m_score_             ;
    unsigned int  m_color_             ;
    unsigned int  m_speed_             ;
    unsigned int  m_kill_counter_      ;
    unsigned int  m_move_counter_      ;
    unsigned int  m_score_offset_      ;
    unsigned char m_direction_         ;
    unsigned char m_old_direction_     ;
    unsigned char m_head_              ;
    bool          m_got_energizer_     ;
    bool          m_check_to_unpause_  ;
    std::clock_t  m_timer_             ;
    std::clock_t  m_timer_on_pause_    ;

    std::shared_ptr<ConsoleSettingsHandler> m_console_handler_;
    Game* m_game_instance_;
};
#endif //PACMAN_H