#pragma once
#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <Windows.h>
#include <iostream>
#include <memory>

#include "constants.h"
#include "KeyboardInput.h"
#include "ConsoleSettingsHandler.h"

using namespace Game_Constants;
class ConsoleSettingsHandler;

class GameMenu : public Keyboard
{
    std::shared_ptr<ConsoleSettingsHandler> m_console_handler;
    int m_choise;
    int m_cursor_position;

public:
    GameMenu(std::shared_ptr<ConsoleSettingsHandler> console_handler);
    ~GameMenu();

    void renderMenu();
    void makeChoise();
    void moveCursorUp();
    void moveCursorDown();
    void setChoise(const int ch) { m_choise = ch;   }
    const int getChoise()        { return m_choise; }
};

#endif //GAMEMENU_H