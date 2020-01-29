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
#include "ConsoleSettingsHandler.h"
#include "CareTakerGhost.h"
#include "constants.h"

using namespace game_constants;

class Game;
class CareTakerGhost;
class Memento;

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
    void       setColor           (const Ghost_Name ghost_name);


    inline const int        getPos_X()                            { return m_x_;                     }
    inline const int        getPos_Y()                            { return m_y_;                     }
    inline const int        getColor()                            { return m_color_;                 }
    inline const char       getDirection()                        { return 0;                        }
    inline const Mode       getMode()                             { return m_current_mode_;          }
    inline const Ghost_Name getName()                             { return m_name_;                  }

    inline void             setMode(const Mode mode)              { m_current_mode_ = mode;          }
    inline void             setHead(const char head)              { m_head_ = head;                  }
    inline void             setPrevMode(const Mode mode)          { m_prev_mode_ = mode;             }
    inline void             setColor(const int color)             { m_color_ = color;                }
    inline void             setPos_X(const int x_pos)             { m_x_ = x_pos;                    }
    inline void             setPos_Y(const int y_pos)             { m_y_ = y_pos;                    }
    inline void             setDirection(const char direction)    { m_direction_ = direction;        }
    inline void             setOldDirection(const char direction) { m_old_direction_ = direction;    }

    inline const double getTimeInWait() { return (std::clock() - m_timer_) / (double)CLOCKS_PER_SEC; }

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