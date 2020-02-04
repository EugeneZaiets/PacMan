#include <memory>

#include "ConsoleSettingsHandler.h"
#include "Game.h"
#include "CareTakerGhost.h"

int main() 
{
    auto handler = std::make_shared<ConsoleSettingsHandler>();
    if (handler == 0)   
        return NULL_POINTER_ERROR;

    std::shared_ptr<Game> game(Game::getInstance(handler));
    if (game == 0) 
        return NULL_POINTER_ERROR;

    game->start();
    return 0;
}