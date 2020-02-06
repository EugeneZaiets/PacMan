#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <memory>
#include <ctime>
#include <vector>

#include <Windows.h>
#include <conio.h>

#include "KeyboardInput.h"
#include "GameMenu.h"
#include "ScoreKeeper.h"
#include "Renderer.h"
#include "PacMan.h"
#include "Ghost.h"

using namespace std;
using namespace game_constants;

class PacMan;
class Ghost;

class Game : public Keyboard
{
public:

    static std::shared_ptr<Game> getInstance
    (
        std::shared_ptr<ConsoleSettingsHandler> console_handler
    );
    ~Game();

    void       start             ();
    void       pause             ();
    void       decreasePointsNum ()                                           { --m_points_num_;             }
    const int  getPointsNum      ()                                           { return m_points_num_;        }
    void       setPointsNum      (const int num)                              { m_points_num_ = num;         }
    void       setCharOfMap      (const int x, const int y, const char print) { m_MapToPrint_[y][x] = print; }
    const char getCharOfBuffer   (const int x, const int y)                   { return m_MapToPrint_[y][x];  }

    const double getTime()
    { return (std::clock() - m_timer_) / (double)CLOCKS_PER_SEC; }

private:

    double m_seconds_in_boost_by_level_                        ;
    double m_seconds_to_mode_change_                           ;
    int    m_points_num_                                       ;
    int    m_level_counter_                                    ;
    int    m_number_of_ghosts_                                 ;
    char   m_MapToPrint_[Y_SIZE][X_SIZE]                       ;
    char   m_substring_[11]                                    ;
    bool   m_isPaused_                                         ;
    bool   m_isGameover_                                       ;
    bool   m_check_to_unpause_                                 ;
    std::clock_t m_timer_, m_temp_timer_                       ;
    std::shared_ptr<ConsoleSettingsHandler> m_console_handler_ ;
    std::unique_ptr<GameMenu>               m_menu_            ;
    std::unique_ptr<PacMan>                 m_pacman_          ;
    std::unique_ptr<Ghost>                  m_ghosts_[4]       ;
    std::unique_ptr<ScoreKeeper>            m_score_keeper_    ;
    std::unique_ptr<Renderer>               m_renderer_        ;

    void       render                  ();
    const bool isDead                  ();
    const bool isEndGame               ();
    const bool collisionWithGhost      ();
    void       startNewGame            ();
    void       startGameLoop           ();
    void       startMenuLoop           ();
    void       startLevel              ();
    void       loadLevel               ();
    void       showBestScores          ();
    void       moveGhosts              ();
    void       fillMapToPrint          ();
    void       initializeAllActors     ();
    void       resetMapInCollision     ();
    void       checkPointersToActors   ();
    void       handleTime              ();
    void       handleTimeEnergizer     ();
    void       changeGhostsModeByTimer ();
    void       handleBlinkyFeature     ();
    void       handlePinkyFeature      ();
    void       handleInkyFeature       ();
    void       handleClydeFeature      ();
    void       handleMenuChoise        (const int);
    void       handleGhostsMovement    (const int);
    const bool handleGhostsKill        (const int);
    const bool renderPause             (const bool);
    void       setMazeText             (std::string, int);
    void       setCharFromMap          (const char, const int, const int);

    static std::shared_ptr<Game> m_GameInstance;
    Game
    (
        std::shared_ptr<ConsoleSettingsHandler> console_handler
    );
    Game           (const Game& ) = delete;
    Game           (const Game&&) = delete;
    Game& operator=(Game const& ) = delete;
    Game& operator=(Game const&&) = delete;
};
#endif // GAME_H