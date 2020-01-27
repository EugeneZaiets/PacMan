#include "Game.h"
#include "ConsoleSettingsHandler.h"
#include "CareTakerGhost.h"
#include <memory>

int main() 
{
    auto handler = std::make_shared<ConsoleSettingsHandler>();
    if (handler == 0)   
        return 1;

    //auto caretaker_ghost   = std::make_unique<CareTakerGhost>();
    //if (caretaker_ghost == 0) 
    //   return 1;

    Game* game = Game::getInstance(handler);
    if (game == 0) 
        return 1;

    game->start();
    return 0;
}