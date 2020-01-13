#pragma once
#ifndef SETCONSOLE_H
#define SETCONSOLE_H

#include <Windows.h>

class ConsoleSettingsHandler {
	HANDLE m_Console;
	HANDLE m_Console_Input;
	COORD windowBuffSize;
public:
	ConsoleSettingsHandler();
	~ConsoleSettingsHandler();

	void SetCustomWindowSize(short x, short y);
	void SetCursorVisibility(bool visibility);
	void SetCursorPosition(short x, short y);
	void SetTextColor(int color);
	bool FlushConsoleBuffer();
	void ResetSettingsToDefault();
	HANDLE GetConsoleOutputHandle();
};
#endif //SETCONSOLE_H
