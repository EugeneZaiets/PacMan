#pragma once
#ifndef MEMENTO_H
#define MEMENTO_H

#include "Game.h"
#include "Ghost.h"
#include "PacMan.h"

template <typename RestoreClass>

class Memento
{
    friend class Game;
    friend class Ghost;
    friend class PacMan;

    RestoreClass m_To_Memento;

    void setOriginator(RestoreClass to_memento);
    RestoreClass getOriginator();
public:
    Memento(RestoreClass to_memento);
    ~Memento();
};

#endif //MEMENTO_H