#include "tsequence.h"

unsigned int TSequence::append(const TElement &element)
{
    QSharedPointer<const TElement> e(&element);
    _list.push_back(e);

    return _list.size() - 1;
}

void TSequence::insert(unsigned int index, const TElement &element)
{
    QSharedPointer<const TElement> e(&element);
    _list.insert(_list.begin() + index, e);
}

void TSequence::remove(unsigned int index)
{
    _list.erase(_list.begin() + index);
}

const TElement &TSequence::get_element(unsigned int index) const
{
    return *_list.at(index);
}

void TSequence::clear()
{
    _list.clear();
}

TSequence::TSequence()
    : _list()
{
}

TSequence::TSequence(const TSystem &system)
    : _list()
{
    add(system);
}

static bool seq_sort(const QSharedPointer<const TElement> &a,
                     const QSharedPointer<const TElement> &b)
{
    return a->get_position().z() < b->get_position().z();
}

void TSequence::add(const TSystem &system)
{
    _list.clear();
    add(static_cast<const TContainer&>(system));
    std::sort(_list.begin(), _list.end(), seq_sort);
}

void TSequence::add(const TContainer &c)
{
    GOPTICAL_FOREACH(i, c.get_element_list())
    {
        if (const TContainer *cc = dynamic_cast<const TContainer*>(i->data()))
            add(*cc);
        else
            _list.push_back(*i);
    }
}

std::ostream & operator<<(std::ostream &o, const TSequence &s)
{
    GOPTICAL_FOREACH(i, s._list)
            o << "  " << **i << std::endl;

    return o;
}


