#ifndef TCONICBASE_H
#define TCONICBASE_H

#include "tcurveroc.h"
#include "trotational.h"

class TConicBase : public TRotational, public TCurveRoc
{
public:
    /** get Schwarzschild constant */
    double get_schwarzschild() const;

    /** get eccentricity */
    double get_eccentricity() const;

    /** Adjust radius of curvature to best fit given
      curve. Deformation Won't be changed by this function. See
      Conic::fit() to adjust deformation too.

      @param curve Curve to fit
      @param radius Maximum radius used to get sample points
      @param count Number of sample points to use
  */
    double fit_roc(const TRotational &c, double radius, unsigned int count);

    virtual bool intersect(Math::Vector3 &point, const Math::VectorPair3 &ray) const = 0;
    virtual double sagitta(double r) const = 0;
    virtual double derivative(double r) const = 0;

protected:
    TConicBase(double roc, double sc);

    double m_sh;       // Schwarzschild constant + 1
};

#endif // TCONICBASE_H
