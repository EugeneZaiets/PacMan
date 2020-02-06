#ifndef PACMAN_H
#define PACMAN_H

#include <iostream>
#include <ctime>

#include <Windows.h>
#include <WinUser.h>
#include <conio.h>
#include <math.h>

#include "Actor.h"
#include "Game.h"

using namespace game_constants;

class Game;

class PacMan : public iActor, public Keyboard
{
public:
    PacMan
    (
        std::shared_ptr<ConsoleSettingsHandler> console_handler,
        shared_ptr<Game> game
    );
    ~PacMan();

    void  dead                 ();
    void  Kill                 ();
    void  handlePacmanMovement (const bool);
    void  resetPacMan          (const int, const int);

    const int           getPos_X        ()                { return m_x_;                  }
    const int           getPos_Y        ()                { return m_y_;                  }
    const int           getPrevPos_X    ()                { return m_prev_x_;             }
    const int           getPrevPos_Y    ()                { return m_prev_y_;             }
    const int           getLives        ()                { return m_lives_;              }
    const int           getScore        ()                { return m_score_;              }
    const int           getKillCounter  ()                { return m_kill_counter_;       } 
    const bool          getEnergizerInfo()                { return m_got_energizer_;      }
    const Directions    getDirection    ()                { return m_direction_;          }
    const unsigned char getHead         ()                { return m_head_;               }

    void setDirection     (const Directions direction)    { m_direction_ = direction;     }
    void setOldDirection  (const Directions direction)    { m_old_direction_ = direction; }
    void setPos_X         (const int x_pos)               { m_x_ = x_pos;                 }
    void setPos_Y         (const int y_pos)               { m_y_ = y_pos;                 }
    void setLives         (const int lives)               { m_lives_ = lives;             }
    void setEnergizerInfo (const bool info)               { m_got_energizer_ = info;      }

    const double getTimeInBooste() 
    { return (std::clock() - m_timer_) / (double)CLOCKS_PER_SEC; }

private:

    int           m_x_, m_y_                 ;
    int           m_prev_x_, m_prev_y_       ;
    unsigned int  m_lives_                   ;
    unsigned int  m_score_                   ;
    unsigned int  m_color_                   ;
    unsigned int  m_speed_                   ;
    unsigned int  m_kill_counter_            ;
    unsigned int  m_move_counter_            ;
    unsigned int  m_score_offset_            ;
    Directions    m_direction_               ;
    Directions    m_old_direction_           ;
    unsigned char m_head_                    ;
    bool          m_got_energizer_           ;
    bool          m_check_to_unpause_        ;
    std::clock_t  m_timer_                   ;
    std::clock_t  m_timer_on_pause_          ;
    std::shared_ptr<ConsoleSettingsHandler> 
                          m_console_handler_ ;
    std::shared_ptr<Game> m_game_instance_   ;

    void       move                       ();
    void       moveUp                     ();
    void       moveLeft                   ();
    void       moveDown                   ();
    void       moveRight                  ();
    void       scoreUp                    ();
    void       gainLife                   ();
    void       eatFood                    ();
    Directions getDirectionFromKeyboard   ();
    void       moveWithDirection          (Directions direction);
    const bool isPaused                   (const bool);
    const bool checkCollision             (const Directions);
};
#endif //PACMAN_H