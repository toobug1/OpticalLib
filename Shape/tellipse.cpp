#include "tellipse.h"
#include "Math/VectorPair"
#include "math/Vector"

double TEllipseBase::get_x_radius(void) const
{
    return _xr;
}

double TEllipseBase::get_y_radius(void) const
{
    return _yr;
}

double TEllipseBase::max_radius() const
{
    return std::max(_yr, _xr);
}

double TEllipseBase::min_radius() const
{
    return std::min(_yr, _xr);
}

double TEllipseBase::get_external_xradius() const
{
    return _xr;
}

double TEllipseBase::get_internal_xradius() const
{
    return 0.0;
}

double TEllipseBase::get_xy_ratio() const
{
    return _xy_ratio;
}

TEllipse::TEllipse(double x_radius, double y_radius)
{
    set_radius(x_radius, y_radius);
}

void TEllipseBase::set_radius(double x_radius, double y_radius)
{
    _xr = x_radius;
    _yr = y_radius;
    _xy_ratio = x_radius / y_radius;
    _e2 = Math::square(sqrt(fabs(_xr * _xr - _yr * _yr)) / std::max(_xr, _yr));
}

bool TEllipseBase::inside(const Math::Vector2 &point) const
{
    return (Math::square(point.x()) + Math::square(point.y() * _xy_ratio) <= Math::square(_xr));
}

Math::VectorPair2 TEllipseBase::get_bounding_box() const
{
    Math::Vector2 hs(_xr, _yr);

    return Math::VectorPair2(-hs, hs);
}

double TEllipseBase::get_outter_radius(const Math::Vector2 &dir) const
{
    return _xr > _yr
            ? sqrt(Math::square(_yr) / (1. - _e2 * Math::square(dir.x())))
            : sqrt(Math::square(_xr) / (1. - _e2 * Math::square(dir.y())));
}

