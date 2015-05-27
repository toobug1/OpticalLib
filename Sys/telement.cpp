#include "telement.h"
#include "tsystem.h"

#include <typeinfo>

// TODO

void TElement::print(std::ostream &o) const
{
    o << " [" << id() << "]" << typeid(*this).name() << " at " << get_position();
}

std::ostream & operator<<(std::ostream &o, const TElement &e)
{
    e.print(o);
    return o;
}

unsigned int TElement::id() const
{
    return _system_id;
}


Math::Vector3 TElement::get_position() const
{
    return (_system->get_global_transform(*this)).transform(Math::vector3_0);
}

