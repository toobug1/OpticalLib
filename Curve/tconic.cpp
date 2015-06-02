#include "tconic.h"

#include <gsl/gsl_fit.h>

TConic::TConic(double roc, double sc)
    : TConicBase(roc, sc)
{
}

inline double TConic::derivative(double r) const
{
    // conical section differentiate (computed with Maxima)

    const double s2 = m_sh * Math::square(r);
    const double s3 = sqrt(1 - s2 / Math::square(m_roc));
    const double s4 = 2.0/(m_roc * (s3+1)) + s2/(Math::square(m_roc) * m_roc * s3 *
                                                Math::square(s3 + 1));

    return r * s4;
}

double TConic::sagitta(double r) const
{
    return Math::square(r) / (m_roc * (sqrt( 1 - (m_sh * Math::square(r)) /
                                            Math::square(m_roc)) + 1));
}

bool TConic::intersect(Math::Vector3 &point, const Math::VectorPair3 &ray) const
{
    const double      ax = ray.origin().x();
    const double      ay = ray.origin().y();
    const double      az = ray.origin().z();
    const double      bx = ray.direction().x();
    const double      by = ray.direction().y();
    const double      bz = ray.direction().z();

    /*
    find intersection point between conical section and ray,
    Telescope optics, page 266
  */
    double a = (m_sh * Math::square(bz) + Math::square(by) + Math::square(bx));
    double b = ((m_sh * bz * az + by * ay + bx * ax) / m_roc - bz) * 2.0;
    double c = (m_sh * Math::square(az) + Math::square(ay) + Math::square(ax)) / m_roc - 2.0 * az;

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

        if (m_sh < 0)
            s = -s;

        t = (2 * c) / (s - b);
    }

    if (t <= 0)               // ignore intersection if before ray origin
        return false;

    point = ray.origin() + ray.direction() * t;

    return true;
}

/*
  ellipse and hyperbola equation standard forms:

  ellipse:    (x^2)/(a^2) + (y^2)/(b^2) = 1
  hyperbola:  (x^2)/(a^2) - (y^2)/(b^2) = 1

  with bend point at (0,0):

  ellipse:    ((x-a)^2)/(a^2) + (y^2)/(b^2) = 1
  hyperbola:  ((x-a)^2)/(a^2) - (y^2)/(b^2) = 1

  and eccentricity:

  ellipse:    e = sqrt(1 - (b^2) / (a^2))
  hyperbola:  e = sqrt(1 + (b^2) / (a^2))

  both can be rewritten as:

  y^2 = (e^2 - 1) * x^2 - 2 * a * (e^2 - 1) * x

  Best fit TConic through (0,0)
  ============================

  y^2 / x = (e^2 - 1) * x - 2 * a * (e^2 - 1)

  is a line model of this form:

  Y = C0 + C1 * X

  with

  Y = y^2 / x
  X = x
  C0 = - 2 * a * (e^2 - 1)
  C1 = (e^2 - 1)

  C0 and C1 are found by least squares fit of points (x,y)

  eccentricity can then be computed for all TConic sections with:

  e = sqrt(C1 + 1)
  sc = -C1 - 1

  radius of curvature can be computed:

  a = C0 / (-2 * C1)
  roc = a * (1 + sc);

  once simplified, works for all TConic sections:

  roc = C0 / 2

 */

double TConic::fit(const TRotational &c, double radius, unsigned int count)
{
    double X[count], Y[count];

    double step = radius / (double)count;
    double y = step / 2.0;

    for (unsigned int i = 0; i < count; i++)
    {
        double x = c.sagitta(y);

        Y[i] = Math::square(y) / x;
        X[i] = x;

        y += step;
    }

    double c0, c1, cov00, cov01, cov11, chisq;

    gsl_fit_linear(X, 1, Y, 1, count,
                   &c0, &c1, &cov00, &cov01, &cov11,
                   &chisq);

    m_sh = -c1;
    m_roc = c0 / 2.0;

    return sqrt(chisq / count); // FIXME bad rms error
}


void TConic::set_eccentricity(double e)
{
    m_sh = - Math::square(e) + 1.0;
}

void TConic::set_schwarzschild(double sc)
{
    m_sh = sc + 1.0;
}


