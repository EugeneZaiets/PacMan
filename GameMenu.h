#pragma once
#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <Windows.h>
#include <iostream>
#include <memory>

#include "constants.h"
#include "KeyboardInput.h"
#include "ConsoleSettingsHandler.h"

using namespace game_constants;
class ConsoleSettingsHandler;

class GameMenu : public Keyboard
{
public:
    GameMenu(std::shared_ptr<ConsoleSettingsHandler> console_handler);
    ~GameMenu();

    void      renderMenu();
    void      makeChoise();
    void      moveCursorUp();
    void      moveCursorDown();
    void      setChoise(const int ch);
    const int getChoise();

private:

    std::shared_ptr<ConsoleSettingsHandler> m_console_handler_;
    int m_choise_;
    int m_cursor_position_;
};

#endif //GAMEMENU_H