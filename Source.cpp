#include "Game.h"
#include "SetConsole.h"

int main() {
	ConsoleSettingsHandler* handler = new ConsoleSettingsHandler();
	Game* game = new Game(handler);
	game->Start();
	delete game;
	delete handler;
	return 0;
}