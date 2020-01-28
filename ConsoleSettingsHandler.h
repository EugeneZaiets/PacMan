#ifndef CONSOLESETTINGSHANDLER_H
#define CONSOLESETTINGSHANDLER_H

#include <iostream>

#include <Windows.h>

#include "constants.h"

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
   
    inline void setTextColor(int  color)            { SetConsoleTextAttribute(m_Console_, color);         }
    inline void setCursorPosition(short x, short y) { SetConsoleCursorPosition(m_Console_, { x, y + 2 }); }
    inline HANDLE getConsoleOutputHandle()          { return m_Console_;                                  }
    inline HANDLE getConsoleInputHandle()           { return m_Console_Input_;                            }

private:

    HANDLE m_Console_;
    HANDLE m_Console_Input_;
    COORD  m_windowBuffSize_;
};





#endif //CONSOLESETTINGSHANDLER_H
