#pragma once
#ifndef GAME_H
#define GAME_H

#include "ConsoleSettingsHandler.h"
//#include "constants.h"
#include "PacMan.h"
#include "Ghost.h"
#include "KeyboardInput.h"
#include "Memento.h"
#include "CareTakerGhost.h"
#include "GameMenu.h"

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <memory>
#include <ctime>
#include <vector>

using namespace std;
using namespace game_constants;

class PacMan;
class Ghost;
class ConsoleSettingsHandler;
class GameMenu;

class Game : public Keyboard
{
public:
    static Game* getInstance(std::shared_ptr<ConsoleSettingsHandler> console_handler);

    void         start();
    void         pause();
    void         decreasePointsNum();
    const int    getPointsNum();
    const char   getCharOfBuffer(const int x, const int y);
    const double getTime();
    const double getTime2();
   
    void         setPointsNum   (const int num);
    void         setCharOfMap   (const int x, const int y, const char print);
    void         setCharFromMap (const char, const int, const int);

private:

    static Game* m_GameInstance;

    double m_seconds_in_boost_by_level_  ;
    double m_seconds_to_mode_change_     ;
    int    m_points_num_                 ;
    int    m_level_counter_              ;
    int    m_number_of_ghosts_           ;
    char   m_MapToPrint_[Y_SIZE][X_SIZE] ;
    char   m_substring_[11]              ;
    bool   m_isPaused_                   ;
    bool   m_isGameover_                 ;
    bool   m_check_to_unpause_           ;

    std::clock_t m_timer_                , 
                 m_timer2_               , 
                 m_temp_timer1_          , 
                 m_temp_timer2_          ;

    std::shared_ptr<ConsoleSettingsHandler> m_console_handler_ ;
    std::unique_ptr<GameMenu>               m_menu_            ;
    std::unique_ptr<PacMan>                 m_pacman_          ;
    std::unique_ptr<Ghost>                  m_ghosts_[4]        ;
    
    const bool isDead();
    const bool collisionWithGhost();
    const bool renderPause(const bool);
    void       render();
    void       game_Loop ();
    void       loadLevel ();
    void       moveGhosts();
    void       handleTime();
    void       startLevel();
    void       initializeAllActors();
    void       setMazeText(std::string, int);
    void       resetMapInCollision();
    void       checkPointersToActors();
    void       determinatePositionForModeActivity();

    Game(std::shared_ptr<ConsoleSettingsHandler> console_handler);
    Game           ( const Game&  ) = delete;
    Game           ( const Game&& ) = delete;
    Game& operator=( Game const&  ) = delete;
    Game& operator=( Game const&& ) = delete;
    ~Game();
};
#endif // GAME_H