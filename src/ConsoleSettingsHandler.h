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
    inline void setCursorPosition(int x, int y)     { SetConsoleCursorPosition(m_Console_, { static_cast<short>(x),
                                                                                             static_cast<short>(y + 2) }); }
    inline HANDLE getConsoleOutputHandle()          { return m_Console_;                                  }
    inline HANDLE getConsoleInputHandle()           { return m_Console_Input_;                            }

private:

    HANDLE m_Console_;
    HANDLE m_Console_Input_;
    COORD  m_windowBuffSize_;
};





#endif //CONSOLESETTINGSHANDLER_H
