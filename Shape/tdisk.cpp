#include "tdisk.h"

#include "Math/VectorPair"

TDisk::TDisk(double radius)
{
    _radius = radius;
}

void TDiskBase::set_radius(double r)
{
    _radius = r;
}

double TDiskBase::get_radius(void) const
{
    return _radius;
}

double TDiskBase::max_radius() const
{
    return _radius;
}

double TDiskBase::min_radius() const
{
    return _radius;
}

double TDiskBase::get_external_xradius() const
{
    return _radius;
}

double TDiskBase::get_internal_xradius() const
{
    return 0.0;
}

double TDiskBase::get_xy_ratio() const
{
    return 1.0;
}

bool TDiskBase::inside(const Math::Vector2 &point) const
{
    return (Math::square(point.x()) + Math::square(point.y()) <= Math::square(_radius));
}

Math::VectorPair2 TDiskBase::get_bounding_box() const
{
    Math::Vector2 hs(_radius, _radius);

    return Math::VectorPair2(-hs, hs);
}

double TDiskBase::get_outter_radius(const Math::Vector2 &dir) const
{
    return _radius;
}




