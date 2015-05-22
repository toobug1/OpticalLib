#include "tparabola.h"

TParabola::TParabola(double roc)
    : TConicBase(roc, -1.0)
{
}

double TParabola::sagitta(double r) const
{
    return Math::square(r) / (2.0 * m_roc);
}

double TParabola::derivative(double r) const
{
    return r / m_roc;
}

bool TParabola::intersect(Math::Vector3 &point, const Math::VectorPair3 &ray) const
{
    const double      ax = ray.origin().x();
    const double      ay = ray.origin().y();
    const double      az = ray.origin().z();
    const double      bx = ray.direction().x();
    const double      by = ray.direction().y();
    const double      bz = ray.direction().z();

    /*
    find intersection point between conical section and line,
    Telescope optics, page 266
  */
    double a = (Math::square(by) + Math::square(bx));
    double b = ((by * ay + bx * ax) / m_roc - bz) * 2.0;
    double c = (Math::square(ay) + Math::square(ax)) / m_roc - 2.0 * az;

    double t;

    if (a == 0)
    {
        t = -c / b;
    }
    else
    {
        double d = Math::square(b) - 4.0 * a * c / m_roc;

        if (d < 0)
            return false;               // no intersection

        double s = sqrt(d);

        if (a * bz < 0)
            s = -s;

        t = (2 * c) / (s - b);
    }

    if (t <= 0)               // ignore intersection if before ray origin
        return false;

    point = ray.origin() + ray.direction() * t;

    return true;
}
