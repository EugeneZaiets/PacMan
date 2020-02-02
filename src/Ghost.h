#ifndef GHOST_H
#define GHOST_H

#include <iostream>
#include <vector>
#include <utility>
#include <ctime>

#include <Windows.h>
#include <conio.h>

#include "Actor.h"
#include "Game.h"

using namespace game_constants;

class Game;

class Ghost : public iActor
{
public:
    Ghost
    (
        std::shared_ptr<ConsoleSettingsHandler> console_handler,
        Game* game,
        Ghost_Name his_name
    );
    ~Ghost();

    void       dead                 ();
    void       scared               ();
    void       braved               ();
    void       renderMap            ();
    void       renderGhost          ();
    void       changeModeToOpposite ();
    void       resetModes           (const int name);
    void       resetGhost           (const int x, const int y);
    void       modeActivity         (const int x, const int y, const bool paused);

    const int        getPos_X    ()                  { return m_x_;                  }
    const int        getPos_Y    ()                  { return m_y_;                  }
    const int        getColor    ()                  { return m_color_;              }
    const Mode       getMode     ()                  { return m_current_mode_;       }
    const Mode       getPrevMode ()                  { return m_prev_mode_;          }
    const Ghost_Name getName     ()                  { return m_name_;               }
    const Directions getDirection()                  { return m_direction_;          }

    void setMode        (const Mode mode)            { m_current_mode_ = mode;       }
    void setHead        (const char head)            { m_head_ = head;               }
    void setPrevMode    (const Mode mode)            { m_prev_mode_ = mode;          }
    void setColor       (const int color)            { m_color_ = color;             }
    void setPos_X       (const int x_pos)            { m_x_ = x_pos;                 }
    void setPos_Y       (const int y_pos)            { m_y_ = y_pos;                 }
    void setSpeed       (const int speed)            { m_speed_ = speed;             }
    void setDirection   (const Directions direction) { m_direction_ = direction;     }
    void setOldDirection(const Directions direction) { m_old_direction_ = direction; }

    const int        getClydeCountPos_X  (const int);
    const int        getClydeCountPos_Y  (const int);
    const int        getInkyPos_X        (const int, const int, const Directions);
    const int        getInkyPos_Y        (const int, const int, const Directions);
    void             setColor            (const Ghost_Name);
    void             setPrevModeAfterExit();

    const double getTimeInWait() 
    { return (std::clock() - m_timer_) / (double)CLOCKS_PER_SEC; }

private:

    int           m_x_, m_y_                ;
    int           m_prev_x_, m_prev_y_      ;
    unsigned int  m_color_                  ;
    unsigned int  m_speed_                  ;
    unsigned int  m_move_counter_           ;
    Directions    m_direction_              ;
    Directions    m_old_direction_          ;
    unsigned char m_head_                   ;
    bool          m_check_to_unpause_       ;
    bool          m_first_exit              ;
    Mode          m_current_mode_           ;
    Mode          m_prev_mode_              ;
    std::clock_t  m_timer_                  ;
    std::clock_t  m_timer_on_pause_         ;
    Ghost_Name    m_name_                   ;
    Game*         m_game_instance_          ;
    std::shared_ptr<ConsoleSettingsHandler> 
                          m_console_handler_;

    void       handleExitMode      ();
    void       handleDeadMode      ();
    void       handleWaitMode      ();
    void       handleScatterMode   ();
    Directions getOppositeDirection();

    const bool isPaused                       (const bool);
    void       move                           (const Directions);
    const bool checkCollision                 (const Directions);
    void       handleChaseMode                (const int, const int);
    void       handleFrightenedMode           (const int, const int);
    const int  countDistance                  (const int, const int);
    Directions determineClosestMove           (const int, const int);
    Directions determineFurthestMove          (const int, const int);
    const int  offsetCoordinatesX             (const Directions, const int);
    const int  offsetCoordinatesY             (const Directions, const int);
    void       determineAppropriateDirections (std::vector<Directions>&);
    void       addAppropriateDirection        (std::vector<Directions>&, Directions);
};
#endif //GHOST_H