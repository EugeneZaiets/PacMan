#include "Memento.h"

template<typename RestoreClass>
Memento<RestoreClass>::Memento(RestoreClass to_memento) :
    m_To_Memento(to_memento)
{
}

template<typename RestoreClass>
Memento<RestoreClass>::~Memento()
{
}

template<typename RestoreClass>
void Memento<RestoreClass>::setOriginator(RestoreClass to_memento)
{
    m_To_Memento = to_memento;
}

template<typename RestoreClass>
RestoreClass Memento<RestoreClass>::getOriginator()
{
    return m_To_Memento;
}

