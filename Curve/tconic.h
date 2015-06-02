#ifndef TCONIC_H
#define TCONIC_H

#include "tconicbase.h"

class TConic : public TConicBase
{
public:
    /** Creates a conic curve with given radius of curvature and
      Schwarzschild constant */
    TConic(double roc, double sc);

    /** Set Schwarzschild constant */
    void set_schwarzschild(double sc);

    /** Set eccentricity */
    void set_eccentricity(double e);

    /** Adjust radius of curvature _and_ deformation to best fit given curve

      @param curve Curve to fit
      @param radius Maximum radius used to get sample points
      @param count Number of sample points to use
  */
    double fit(const TRotational &curve, double radius, unsigned int count);

    bool intersect(Math::Vector3 &point, const Math::VectorPair3 &ray) const;
    double sagitta(double r) const;
    double derivative(double r) const;
};

#endif // TCONIC_H
