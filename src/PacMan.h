#ifndef PACMAN_H
#define PACMAN_H

#include <iostream>
#include <ctime>

#include <Windows.h>
#include <WinUser.h>
#include <conio.h>
#include <math.h>

#include "Actor.h"
#include "KeyboardInput.h"
#include "Game.h"
#include "ConsoleSettingsHandler.h"

using namespace game_constants;

class Game;

class PacMan : public iActor, public Keyboard
{
public:
    PacMan
    (
        std::shared_ptr<ConsoleSettingsHandler> console_handler, 
        Game* game
    );
    ~PacMan();

    void  dead();
    void  scoreUp();
    void  gainLife();
    void  renderKill();
    void  renderScore();
    void  renderLives();
    void  renderPacman();
    const char getDirection();

    void       move             (const bool);
    const bool isPaused         (const bool);
    const bool checkCollision   (const char);
    void       resetPacMan      (const int x, const int y);
    void       resetMapAfterKill(const int x, const int y, const int num_elements);

    inline const int    getPos_X()                            { return m_x_;                           }
    inline const int    getPos_Y()                            { return m_y_;                           }
    inline const int    getLives()                            { return m_lives_;                       }
    inline const bool   getEnergizerInfo()                    { return m_got_energizer_;               }

    inline void         setDirection(const char direction)    { m_direction_ = direction;              }
    inline void         setOldDirection(const char direction) { m_old_direction_ = direction;          }
    inline void         setPos_X(const int x_pos)             { m_x_ = x_pos;                          }
    inline void         setPos_Y(const int y_pos)             { m_y_ = y_pos;                          }
    inline void         setLives(const int lives)             { m_lives_ = lives;                      }
    inline void         setEnergizerInfo(const bool info)     { m_got_energizer_ = info;               }

    inline const double getTimeInBooste() { return (std::clock() - m_timer_) / (double)CLOCKS_PER_SEC; }
    
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

    void moveUp();
    void moveLeft();
    void moveDown();
    void moveRight();

    std::shared_ptr<ConsoleSettingsHandler> m_console_handler_;
    Game* m_game_instance_;
};
#endif //PACMAN_H