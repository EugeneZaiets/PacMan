#include "Game.h"
#include "ConsoleSettingsHandler.h"
#include <memory>

int main() 
{
	std::shared_ptr<ConsoleSettingsHandler> handler = std::make_shared<ConsoleSettingsHandler>(); // could be used auto 
	if (handler == 0) return 1;
	Game* game = Game::getInstance(handler);
	game->start();
	return 0;
}