#include "Game.h"
#include "ConsoleSettingsHandler.h"
#include <memory>

int main() 
{
	std::shared_ptr<ConsoleSettingsHandler> handler = std::make_shared<ConsoleSettingsHandler>(); // could be used auto 
	if (handler == 0) return 1;
	std::unique_ptr<Game> game = std::make_unique<Game>(handler);
	if (game == 0) return 1;
	game->Start();
	return 0;
}