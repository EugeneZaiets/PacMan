#include "ConsoleSettingsHandler.h"

ConsoleSettingsHandler::ConsoleSettingsHandler() :
	m_Console(GetStdHandle(STD_OUTPUT_HANDLE)),
	m_Console_Input(GetStdHandle(STD_INPUT_HANDLE)),
	windowBuffSize({ X_GAME_SCREEN_SIZE , Y_GAME_SCREEN_SIZE })
{
}
ConsoleSettingsHandler::~ConsoleSettingsHandler()
{
}
void ConsoleSettingsHandler::CreateGameWindow()
{

	SMALL_RECT windowSize = { 0, 0, X_GAME_SCREEN_SIZE - 1, Y_GAME_SCREEN_SIZE - 1 };
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
void ConsoleSettingsHandler::HanldeCursorVisibility(bool visibility)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	cursorInfo.bVisible = visibility;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
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
