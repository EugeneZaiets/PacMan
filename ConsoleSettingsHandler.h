#pragma once
#ifndef CONSOLESETTINGSHANDLER_H
#define CONSOLESETTINGSHANDLER_H

#include "constants.h"

#include <iostream>
#include <Windows.h>
using namespace std;
using namespace game_constants;

class ConsoleSettingsHandler {
public:
    ConsoleSettingsHandler();
    ~ConsoleSettingsHandler();

    bool   flushConsoleBuffer();
    void   resetSettingsToDefault();
    void   createGameWindow();
    void   hanldeCursorVisibility (bool visibility );
    void   setTextColor           (int  color      );
    void   setCursorPosition      (short x, short y);

    HANDLE getConsoleOutputHandle();
    HANDLE getConsoleInputHandle ();      

private:

    HANDLE m_Console_;
    HANDLE m_Console_Input_;
    COORD  m_windowBuffSize_;
};
#endif //CONSOLESETTINGSHANDLER_H
