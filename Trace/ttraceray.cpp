#include "ttraceray.h"
#include "Sys/telement.h"
#include "Material/tglass.h"
#include "Math/Vector"

#include <limits>


TTraceRay::TTraceRay()
    : TRay(),
      _len(std::numeric_limits<double>::max()),
      _creator(0),
      _parent(0),
      _child(0),
      _lost(true)
{
}

TTraceRay::TTraceRay(const TRay &r)
    : TRay(r),
      _len(std::numeric_limits<double>::max()),
      _creator(0),
      _parent(0),
      _child(0),
      _lost(true)
{
}

void TTraceRay::add_generated(TTraceRay *r)
{
    assert(!r->_parent);
    r->_parent = this;
    r->_next = _child;
    _child = r;
}

void TTraceRay::set_intercept(const TElement &e, const Math::Vector3 &point)
{
    _i_element = (TElement*)&e;
    _point = point;
    _lost = false;
}

TTraceRay * TTraceRay::get_parent() const
{
    return _parent;
}

TTraceRay * TTraceRay::get_next_child() const
{
    return _next;
}

TTraceRay * TTraceRay::get_first_child() const
{
    return _child;
}

const Math::Vector3 & TTraceRay::get_intercept_point() const
{
    return _point;
}

bool TTraceRay::is_lost() const
{
    return _lost;
}

TElement & TTraceRay::get_intercept_element() const
{
    return *_i_element;
}

Math::Vector3 TTraceRay::get_position(const TElement &e) const
{
    return _creator->get_transform_to(e).transform(origin());
}

Math::Vector3 TTraceRay::get_direction(const TElement &e) const
{
    return _creator->get_transform_to(e).transform_linear(direction());
}

Math::Vector3 TTraceRay::get_position() const
{
    return _creator->get_global_transform().transform(origin());
}

Math::Vector3 TTraceRay::get_direction() const
{
    return _creator->get_global_transform().transform_linear(direction());
}


