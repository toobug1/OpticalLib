#include "tellipticalring.h"

#include "Math/Vector"
#include "Math/VectorPair"

double TEllipticalRingBase::get_x_radius(void) const
{
    return _xr;
}

double TEllipticalRingBase::get_y_radius(void) const
{
    return _yr;
}

double TEllipticalRingBase::get_x_hole_radius(void) const
{
    return _xhr;
}

double TEllipticalRingBase::get_y_hole_radius(void) const
{
    return _xhr / _xy_ratio;
}

double TEllipticalRingBase::max_radius() const
{
    return std::max(_yr, _xr);
}

double TEllipticalRingBase::min_radius() const
{
    return std::min(_yr, _xr);
}

double TEllipticalRingBase::get_external_xradius() const
{
    return _xr;
}

double TEllipticalRingBase::get_internal_xradius() const
{
    return _xhr;
}

double TEllipticalRingBase::get_xy_ratio() const
{
    return _xy_ratio;
}

TEllipticalRing::TEllipticalRing(double x_radius, double y_radius, double x_hole_radius)
{
    set_radius(x_radius, y_radius, x_hole_radius);
}

void TEllipticalRingBase::set_radius(double x_radius, double y_radius, double x_hole_radius)
{
    assert(x_radius > x_hole_radius);

    _xr = x_radius;
    _xhr = x_hole_radius;
    _yr = y_radius;
    _xy_ratio = x_radius / y_radius;
    _e2 = Math::square(sqrt(fabs(_xr * _xr - _yr * _yr)) / std::max(_xr, _yr));
}

bool TEllipticalRingBase::inside(const Math::Vector2 &point) const
{
    double d = sqrt(Math::square(point.x()) + Math::square(point.y() * _xy_ratio));

    return (d <= _xr && d >= _xhr);
}

Math::VectorPair2 TEllipticalRingBase::get_bounding_box() const
{
    Math::Vector2 hs(_xr, _yr);

    return Math::VectorPair2(-hs, hs);
}

double TEllipticalRingBase::get_outter_radius(const Math::Vector2 &dir) const
{
    return _xr > _yr
            ? sqrt(Math::square(_yr) / (1. - _e2 * Math::square(dir.x())))
            : sqrt(Math::square(_xr) / (1. - _e2 * Math::square(dir.y())));
}

double TEllipticalRingBase::get_hole_radius(const Math::Vector2 &dir) const
{
    return _xr > _yr
            ? sqrt(Math::square(_xhr / _xy_ratio) / (1. - _e2 * Math::square(dir.x())))
            : sqrt(Math::square(_xhr) / (1. - _e2 * Math::square(dir.y())));
}

