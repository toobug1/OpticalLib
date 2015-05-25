#include "trectangle.h"
#include "Math/VectorPair"
#include "Trace/tdistribution.h"

#include <limits>


TRectangle::TRectangle(double sqsize)
    : _halfsize(Math::Vector2(sqsize / 2., sqsize / 2.))
{
}

TRectangle::TRectangle(double width, double height)
    : _halfsize(Math::Vector2(width / 2., height / 2.))
{
}

double TRectangle::max_radius() const
{
    return _halfsize.len();
}

double TRectangle::min_radius() const
{
    return std::min(_halfsize.x(), _halfsize.y());
}

bool TRectangle::inside(const Math::Vector2 &point) const
{
    return (fabs(point.x()) <= _halfsize.x() && fabs(point.y()) <= _halfsize.y());
}

void TRectangle::get_pattern(const Math::Vector2::put_delegate_t  &f,
                             const TDistribution &d,
                             bool unobstructed) const
{
    static const double epsilon = 1e-8;
    const Math::Vector2 hs = _halfsize * d.get_scaling();
    const Math::Vector2 step = hs / (double)(d.get_radial_density() / 2);

    switch (d.get_pattern())
    {
    case Trace::MeridionalDist: {

        f(Math::Vector2(0, 0));

        for (double y = step.y(); y < hs.y() + epsilon; y += step.y())
        {
            f(Math::Vector2(0, y));
            f(Math::Vector2(0, -y));
        }
        break;
    }

    case Trace::SagittalDist: {

        f(Math::Vector2(0, 0));

        for (double x = step.x(); x < hs.x() + epsilon; x += step.x())
        {
            f(Math::Vector2(x, 0));
            f(Math::Vector2(-x, 0));
        }
        break;
    }

    case Trace::CrossDist: {

        f(Math::Vector2(0, 0));

        for (double x = step.x(); x < hs.x() + epsilon; x += step.x())
        {
            f(Math::Vector2(x, 0));
            f(Math::Vector2(-x, 0));
        }

        for (double y = step.y(); y < hs.y() + epsilon; y += step.y())
        {
            f(Math::Vector2(0, y));
            f(Math::Vector2(0, -y));
        }
        break;
    }

    case Trace::DefaultDist:
    case Trace::SquareDist: {
        double x, y;

        f(Math::Vector2(0, 0));

        for (x = step.x(); x < hs.x() + epsilon; x += step.x())
            for (y = step.y(); y < hs.y() + epsilon; y += step.y())
            {
                f(Math::Vector2(x, y));
                f(Math::Vector2(-x, y));
                f(Math::Vector2(x, -y));
                f(Math::Vector2(-x, -y));
            }

        for (x = step.x(); x < hs.x() + epsilon; x += step.x())
        {
            f(Math::Vector2(x, 0));
            f(Math::Vector2(-x, 0));
        }

        for (y = step.y(); y < hs.y() + epsilon; y += step.y())
        {
            f(Math::Vector2(0, y));
            f(Math::Vector2(0, -y));
        }
        break;
    }

    default:
        TShapeBase::get_pattern(f, d, unobstructed);
    }
}

Math::VectorPair2 TRectangle::get_bounding_box() const
{
    return Math::VectorPair2(-_halfsize, _halfsize);
}

unsigned int TRectangle::get_contour_count() const
{
    return 1;
}

inline Math::Vector2 TRectangle::get_step(double resolution) const
{
    Math::Vector2 step;

    for (unsigned int i = 0; i < 2; i++)
    {
        if (resolution > _halfsize[i])
            step[i] = _halfsize[i];
        else
            step[i] = _halfsize[i] / round(_halfsize[i] / resolution);
    }

    return step;
}

void TRectangle::get_contour(unsigned int contour, const Math::Vector2::put_delegate_t  &f, double resolution) const
{
    static const double epsilon = 1e-8;
    assert(contour < 1);

    Math::Vector2 step = get_step(resolution);

    double x, y;

    for (x = - _halfsize.x(); x < _halfsize.x() - epsilon; x += step.x())
        f(Math::Vector2(x, - _halfsize.y()));

    for (y = - _halfsize.y(); y < _halfsize.y() - epsilon; y += step.y())
        f(Math::Vector2(_halfsize.x(), y));

    for (x = _halfsize.x(); x > - _halfsize.x() + epsilon; x -= step.x())
        f(Math::Vector2(x, _halfsize.y()));

    for (y = _halfsize.y(); y > - _halfsize.y() + epsilon; y -= step.y())
        f(Math::Vector2(- _halfsize.x(), y));
}

void TRectangle::get_triangles(const Math::Triangle<2>::put_delegate_t  &f, double resolution) const
{
    static const double epsilon = 1e-8;
    Math::Vector2 step = get_step(resolution);

    for (unsigned int i = 0; i < 2; i++)
    {
        if (resolution > _halfsize[i])
            step[i] = _halfsize[i];
        else
            step[i] = _halfsize[i] / round(_halfsize[i] / resolution);
    }

    for (double x = 0; x < _halfsize.x() - epsilon; x += step.x())
        for (double y = 0; y < _halfsize.y() - epsilon; y += step.y())
        {
            Math::Vector2 a(x, y);
            Math::Vector2 b(x + step.x(), y);
            Math::Vector2 c(x, y + step.y());
            Math::Vector2 d(x + step.x(), y + step.y());

            f(Math::Triangle<2>(b, a, c));
            f(Math::Triangle<2>(d, b, c));
            f(Math::Triangle<2>(-b, -a, -c));
            f(Math::Triangle<2>(-d, -b, -c));

            a.x() = -a.x();
            b.x() = -b.x();
            c.x() = -c.x();
            d.x() = -d.x();

            f(Math::Triangle<2>(a, b, c));
            f(Math::Triangle<2>(b, d, c));
            f(Math::Triangle<2>(-a, -b, -c));
            f(Math::Triangle<2>(-b, -d, -c));
        }
}

double TRectangle::get_outter_radius(const Math::Vector2 &dir) const
{
    const Math::Vector2 &e = (fabs(dir.x() / dir.y()) < _halfsize.x() / _halfsize.y())
            ? Math::vector2_10 : Math::vector2_01;

    return (Math::VectorPair2(_halfsize, e)
            .ln_intersect_ln(Math::VectorPair2(Math::vector2_0, dir))).len();
}


