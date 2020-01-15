#include "Game.h"
#include "ConsoleSettingsHandler.h"
#include <memory>

int main() 
{
	ConsoleSettingsHandler* handler = new ConsoleSettingsHandler();
	Game* game = new Game(handler);
	game->Start();
	delete game;
	delete handler;
	return 0;
}