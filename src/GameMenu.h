#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <iostream>
#include <memory>

#include <Windows.h>

#include "KeyboardInput.h"
#include "ConsoleSettingsHandler.h"
#include "constants.h"

using namespace game_constants;
class ConsoleSettingsHandler;

class GameMenu : public Keyboard
{
public:
    GameMenu
    (
        std::shared_ptr<ConsoleSettingsHandler> console_handler
    );
    ~GameMenu();

    void      renderMenu     ();
    void      makeChoise     ();
    void      moveCursorUp   ();
    void      moveCursorDown ();
    void      setChoise      (const int ch) { m_choise_ = ch;   }
    const int getChoise      ()             { return m_choise_; }

private:

    std::shared_ptr<ConsoleSettingsHandler> 
        m_console_handler_ ;
    int m_choise_          ;
    int m_cursor_position_ ;
};

#endif //GAMEMENU_H