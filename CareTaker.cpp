#include "CareTaker.h"


template<typename RestoreClass>
CareTaker<RestoreClass>::CareTaker()
{
}

template<typename RestoreClass>
CareTaker<RestoreClass>::~CareTaker()
{
}

template<typename RestoreClass>
void CareTaker<RestoreClass>::setMemento(std::unique_ptr<Memento<RestoreClass>> memento)
{
    m_listMemento.push_back(std::move(memento));
}

template<typename RestoreClass>
std::unique_ptr<Memento<RestoreClass>> CareTaker<RestoreClass>::getMemento()
{
    std::unique_ptr<Memento<RestoreClass>> last_element_in_list;
    last_element_in_list = std::move(m_listMemento.back());
    m_listMemento.pop_back();
    return last_element_in_list;
}

