#pragma once
#ifndef CONSOLESETTINGSHANDLER_H
#define CONSOLESETTINGSHANDLER_H

#include "constants.h"

#include <iostream>
#include <Windows.h>
using namespace std;
using namespace Game_Constants;

class ConsoleSettingsHandler {
	HANDLE m_Console;
	HANDLE m_Console_Input;
	COORD windowBuffSize;
public:
	ConsoleSettingsHandler();
	~ConsoleSettingsHandler();

	bool flushConsoleBuffer();
	void resetSettingsToDefault();
	void createGameWindow();
	void hanldeCursorVisibility(bool visibility);

	void setCursorPosition(short x, short y)      { SetConsoleCursorPosition(m_Console, { x, y + 2 }); }
	void setTextColor(int color)                  { SetConsoleTextAttribute(m_Console, color);         }
	HANDLE getConsoleOutputHandle()               { return m_Console;                                  }
    HANDLE getConsoleInputHandle()                { return m_Console_Input;                            }
};
#endif //CONSOLESETTINGSHANDLER_H
