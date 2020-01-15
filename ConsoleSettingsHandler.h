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

	bool FlushConsoleBuffer();
	void ResetSettingsToDefault();
	void CreateGameWindow();
	void HanldeCursorVisibility(bool visibility);

	void SetCursorPosition(short x, short y)      { SetConsoleCursorPosition(m_Console, { x, y + 2 }); }
	void SetTextColor(int color)                  { SetConsoleTextAttribute(m_Console, color);         }
	HANDLE GetConsoleOutputHandle()               { return m_Console;                                  }
};
#endif //CONSOLESETTINGSHANDLER_H
