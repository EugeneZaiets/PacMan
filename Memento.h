#pragma once
#ifndef MEMENTO_H
#define MEMENTO_H

#include "Game.h"
#include "Ghost.h"
#include "PacMan.h"

class Memento
{
    friend class Ghost;
    Ghost m_To_Memento;

    void setOriginator(Ghost to_memento);
    Ghost getOriginator();
public:
    Memento(Ghost to_memento);
    ~Memento();
};

#endif //MEMENTO_H