#include "tring.h"

#include "Math/Vector"
#include "Math/VectorPair"

TRing::TRing(double radius, double hole_radius)
{
    set_radius(radius, hole_radius);
}

void TRingBase::set_radius(double radius, double hole_radius)
{
    assert(radius > hole_radius);

    _radius = radius;
    _hole_radius = hole_radius;
}

double TRingBase::get_radius(void) const
{
    return _radius;
}

double TRingBase::get_hole_radius(void) const
{
    return _hole_radius;
}

double TRingBase::max_radius() const
{
    return _radius;
}

double TRingBase::min_radius() const
{
    return _radius;
}

double TRingBase::get_external_xradius() const
{
    return _radius;
}

double TRingBase::get_internal_xradius() const
{
    return _hole_radius;
}

double TRingBase::get_xy_ratio() const
{
    return 1.0;
}

bool TRingBase::inside(const Math::Vector2 &point) const
{
    double d = Math::square(point.x()) + Math::square(point.y());

    return d <= Math::square(_radius) && d >= Math::square(_hole_radius);
}

Math::VectorPair2 TRingBase::get_bounding_box() const
{
    Math::Vector2 hs(_radius, _radius);

    return Math::VectorPair2(-hs, hs);
}

double TRingBase::get_outter_radius(const Math::Vector2 &dir) const
{
    return _radius;
}

double TRingBase::get_hole_radius(const Math::Vector2 &dir) const
{
    return _hole_radius;
}


