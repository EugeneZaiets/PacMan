#include "SetConsole.h"
#include "constants.h"

#include <iostream>
#include <Windows.h>
using namespace std;

ConsoleSettingsHandler::ConsoleSettingsHandler() {
	m_Console		= GetStdHandle(STD_OUTPUT_HANDLE);
	m_Console_Input = GetStdHandle(STD_INPUT_HANDLE);
}
ConsoleSettingsHandler::~ConsoleSettingsHandler(){}
void ConsoleSettingsHandler::SetCustomWindowSize(short x, short y)
{
	windowBuffSize		  = { x, y };
	SMALL_RECT windowSize = { 0, 0, X_SCREEN_SIZE - 1, Y_SCREEN_SIZE - 1 };
	if (!SetConsoleWindowInfo(m_Console, TRUE, &windowSize))
	{
		std::cout << "SetConsoleWindowInfo failed with error " << GetLastError() << endl;
		system("pause");
		exit(WINDOW_INFO_FAIL);
	}
	if (!SetConsoleScreenBufferSize(m_Console, windowBuffSize))
	{
		cout << "SetConsoleScreenBufferSize failed with error " << GetLastError() << endl;
		system("pause");
		exit(CONSOLE_BUFFERSIZE_FAIL);
	}
}
void ConsoleSettingsHandler::SetCursorVisibility(bool visibility) {
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	cursorInfo.bVisible = visibility;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
void ConsoleSettingsHandler::SetCursorPosition(short x, short y) {
	SetConsoleCursorPosition(m_Console, {x, y + 2});
}
void ConsoleSettingsHandler::SetTextColor(int color)
{
	SetConsoleTextAttribute(m_Console, color);
}
bool ConsoleSettingsHandler::FlushConsoleBuffer()
{
	return FlushConsoleInputBuffer(this->m_Console_Input);
}
void ConsoleSettingsHandler::ResetSettingsToDefault()
{
	SetConsoleOutputCP(866);
	SetConsoleTextAttribute(m_Console, WHITE);
}
HANDLE ConsoleSettingsHandler::GetConsoleOutputHandle()
{
	return this->m_Console;
}

