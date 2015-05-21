#include "tconicbase.h"

#include <gsl/gsl_fit.h>

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

  Best fit conic through (0,0) with fixed eccentricity
  ====================================================

  * Ellipse, Circle, Hyperbola:

  y^2 - (e^2 - 1) * x^2 = -2 * a * (e^2 - 1) * x

  is a line model of this form:

  Y = C1 * X

  with

  Y = y^2 - (e^2 - 1) * x^2
  X = -2 * (e^2 - 1) * x
  C1 = a

  C1 is found by least squares fit of points (x,y)

  radius of curvature can be computed:

  roc = a * (1 + sc);
  roc = C1 * sc

  * Parabola:

  y^2 = 4 * a * x

  is used instead, with

  Y = y^2
  X = 4 * x
  C1 = a

  radius of curvature can be computed:

  roc = 2 * C1

*/

double TConicBase::fit_roc(const TRotational &c, double radius, unsigned int count)
{
  double X[count], Y[count];

  double step = radius / (double)count;
  double y = step / 2.0;
  double c1, cov11, chisq;

  if (m_sh != 0.0)
    {
      for (unsigned int i = 0; i < count; i++)
        {
          double x = c.sagitta(y);

          Y[i] = Math::square(y) + m_sh * Math::square(x);
          X[i] = 2.0 * m_sh * x;

          y += step;
        }

      gsl_fit_mul(X, 1, Y, 1, count, &c1, &cov11, &chisq);

      m_roc = c1 * m_sh;
    }
  else
    { // Parabola special case
      for (unsigned int i = 0; i < count; i++)
        {
          double x = c.sagitta(y);

          Y[i] = Math::square(y);
          X[i] = 4.0 * x;

          y += step;
        }

      gsl_fit_mul(X, 1, Y, 1, count, &c1, &cov11, &chisq);

      m_roc = 2.0 * c1;
    }

  return sqrt(chisq / count); // FIXME bad rms error
}

TConicBase::TConicBase(double roc, double sc)
  : TCurveRoc(roc), m_sh(sc + 1)
{
}

double TConicBase::get_eccentricity() const
{
  return sqrt(- m_sh + 1.0);
}

double TConicBase::get_schwarzschild() const
{
  return m_sh - 1.0;
}
