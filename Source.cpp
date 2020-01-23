#include "Game.h"
#include "ConsoleSettingsHandler.h"
#include "CareTakerGhost.h"
#include <memory>

int main() 
{
    auto handler           = std::make_shared<ConsoleSettingsHandler>(); // could be used auto 
    auto caretaker_ghost   = std::make_unique<CareTakerGhost>();
 
	if (!handler)         return 1;
    if (!caretaker_ghost) return 1;


	Game* game = Game::getInstance(handler); 
    if (!game) return 1;
	game->start();
	return 0;
}