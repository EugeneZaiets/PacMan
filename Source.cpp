#include "Game.h"
#include "ConsoleSettingsHandler.h"
#include "CareTaker.h"
#include <memory>

int main() 
{
    auto handler          = std::make_shared<ConsoleSettingsHandler>(); // could be used auto 
    auto caretaker_game   = std::make_unique<CareTaker<Game>>();
    auto caretaker_pacman = std::make_unique<CareTaker<PacMan>>();
    auto caretaker_ghost  = std::make_unique<CareTaker<Ghost>>();
	if (!handler)          return 1;
    if (!caretaker_game)   return 1;
    if (!caretaker_pacman) return 1;
    if (!caretaker_ghost)  return 1;

	Game* game = Game::getInstance(handler); 
    if (!game) return 1;
	game->start();
	return 0;
}