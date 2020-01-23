#pragma once
#ifndef CARETAKER_H
#define CARETAKER_H

#include "Memento.h"
#include "Game.h"
#include "PacMan.h"
#include "Ghost.h"

#include <list>
#include <memory>

template<typename RestoreClass>

class CareTaker
{
    std::list <std::unique_ptr<RestoreClass>> m_listMemento;
public:
    CareTaker();
    ~CareTaker();

    void setMemento(std::unique_ptr<Memento<RestoreClass>> memento);
    std::unique_ptr<Memento<RestoreClass>> getMemento();
};

#endif //CARETAKER_H