#include "tregularpolygon.h"

#include "Math/Vector"
#include "Math/VectorPair"

double TRegularPolygon::max_radius() const
{
    return _radius;
}

double TRegularPolygon::min_radius() const
{
    return _i_radius;
}

TRegularPolygon::TRegularPolygon(double radius, unsigned int edge_cnt, double angle)
    : _radius(radius),
      _edge_cnt(edge_cnt),
      _angle(Math::degree2rad(angle))
{
    assert(_edge_cnt >= 3);
    _a_step = 2.0 * M_PI / (double)_edge_cnt;
    _i_radius = cos(M_PI / (double)_edge_cnt) * _radius;
}

Math::VectorPair2 TRegularPolygon::get_edge(const Math::Vector2 &point) const
{
    // find sector start angle
    double a = Math::lp_floor(atan2(point.y(), point.x()) - _angle, (double)_edge_cnt / (2.0 * M_PI));

    // find sector edge coordinates
    return Math::VectorPair2(_radius * cos(_angle + a), _radius * sin(_angle + a),
                             _radius * cos(_angle + a + _a_step), _radius * sin(_angle + a + _a_step));
}

bool TRegularPolygon::inside(const Math::Vector2 &point) const
{
    double d = Math::square(point.x()) + Math::square(point.y());

    // check against circumscribed circle
    if (d > Math::square(_radius))
        return false;

    // check against inscribed circle
    if (d < Math::square(_i_radius))
        return true;

    Math::VectorPair2 e(get_edge(point));

    return ((point.y() - e.y0()) * (e.x1() - e.x0()) -
            (point.x() - e.x0()) * (e.y1() - e.y0()) > 0);
}

Math::VectorPair2 TRegularPolygon::get_bounding_box() const
{
    // bounding box could be narrower FIXME?
    return Math::VectorPair2(Math::Vector2(_radius, _radius),
                             Math::Vector2(-_radius, -_radius));
}

unsigned int TRegularPolygon::get_contour_count() const
{
    return 1;
}

double TRegularPolygon::get_radial_step(double resolution) const
{
    if (resolution > _radius)
        resolution = _radius;

    return _radius / ceil(_radius / resolution);
}

void TRegularPolygon::get_contour(unsigned int contour, const Math::Vector2::put_delegate_t  &f, double resolution) const
{
    // FIXME use resolution

    static const double epsilon = 1e-8;

    assert(contour < 1);

    for (double a = 0; a < 2 * M_PI - epsilon; a += _a_step)
        f(Math::Vector2(cos(_angle + a) * _radius, sin(_angle + a) * _radius));
}

void TRegularPolygon::get_triangles(const Math::Triangle<2>::put_delegate_t &f, double resolution) const
{
    static const double epsilon = 1e-8;
    const double rstep = get_radial_step(resolution);
    const double astep1 = 2.0 * M_PI / (double)_edge_cnt;

    for (double r = 0 ; r < _radius - epsilon; r += rstep)
    {
        for (double a1 = 0; a1 < 2.0 * M_PI - epsilon; a1 += astep1)
        {
            Math::Vector2 a(cos(_angle + a1), sin(_angle + a1));
            Math::Vector2 aa(a * r);
            Math::Vector2 ab(a * (r + rstep));

            Math::Vector2 b(cos(_angle + a1 + astep1), sin(_angle + a1 + astep1));
            Math::Vector2 ba(b * r);
            Math::Vector2 bb(b * (r + rstep));

            f(Math::Triangle<2>(aa, ba, bb));
            f(Math::Triangle<2>(bb, ab, aa));
        }

    }

}

double TRegularPolygon::get_outter_radius(const Math::Vector2 &dir) const
{
    Math::VectorPair2 e(get_edge(dir));
    Math::VectorPair2 d(Math::vector2_0, dir);

    return e.seg_intersect_seg<true, true>(d).len();
}
