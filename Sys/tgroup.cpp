#include "tgroup.h"
#include "Sys/tsystem.h"
#include "Io/trenderer.h"

TGroup::TGroup(const Math::VectorPair3 &p)
    : TElement(p),
      TContainer()
{
}

TGroup::~TGroup()
{
    remove_all();
}

void TGroup::added(TElement &e)
{
    // register new element if TGroup is part of a system
    if (_system)
        e.system_register(*_system);
}

void TGroup::removed(TElement &e)
{
    // unregister removed element if TGroup is part of a system
    if (_system)
        e.system_unregister();
}

void TGroup::system_register(TSystem &s)
{
    TElement::system_register(s);

    // register all children elements
    GOPTICAL_FOREACH(i, get_element_list())
            (*i)->system_register(s);
}

void TGroup::system_unregister()
{
    assert(_system);

    // unregister all children elements
    GOPTICAL_FOREACH(i, get_element_list())
            (*i)->system_unregister();

    TElement::system_unregister();
}

void TGroup::system_moved()
{
    GOPTICAL_FOREACH(i, get_element_list())
            (*i)->system_moved();

    TElement::system_moved();
}

Math::VectorPair3 TGroup::get_bounding_box() const
{
    return TContainer::get_bounding_box();
}

void TGroup::draw_2d_e(TRenderer &r, const TElement *ref) const
{
    GOPTICAL_FOREACH(i, get_element_list())
            r.draw_element_2d(**i, ref);
}

void TGroup::draw_3d_e(TRenderer &r, const TElement *ref) const
{
    GOPTICAL_FOREACH(i, get_element_list())
            r.draw_element_3d(**i, ref);
}

