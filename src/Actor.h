#ifndef ACTOR_H
#define ACTOR_H

#include "constants.h"
using namespace game_constants;

class iActor
{
public:
    virtual const int        getPos_X()                       = 0;
    virtual const int        getPos_Y()                       = 0;
    virtual const Directions getDirection()                   = 0;
    virtual void             setDirection  (const Directions) = 0;
    virtual const bool       checkCollision(const Directions) = 0;
    virtual void             setPos_X      (const  int)       = 0;
    virtual void             setPos_Y      (const  int)       = 0;
    virtual void             dead()                           = 0;
    virtual ~iActor(){};
};
#endif //ACTOR_H