#pragma once
#ifndef CARETAKERGHOST_H
#define CARETAKERGHOST_H

#include "Memento.h"

#include <list>
#include <memory>

class CareTakerGhost
{
    std::list <std::unique_ptr<Memento>> m_listMemento;
public:
    CareTakerGhost();
    ~CareTakerGhost();

    void setMemento(std::unique_ptr<Memento> memento);
    std::unique_ptr<Memento> getMemento();
};

#endif //CARETAKERGHOST_H