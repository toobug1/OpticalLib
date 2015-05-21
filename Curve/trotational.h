#ifndef TROTATIONAL_H
#define TROTATIONAL_H

#include <gsl/gsl_deriv.h>

#include "tcurvebase.h"
#include "Math/vector.hh"


class TRotational : public TCurveBase
{
public:
    TRotational();

    virtual void normal(Math::Vector3 &normal, const Math::Vector3 &point) const;

    /** Get curve sagitta at specified distance from origin.
      @param r distance from curve origin (0, 0)
  */
    virtual double sagitta(double r) const = 0;

    /** Get curve derivative at specified distance from origin.
      @param r distance from curve origin (0, 0)
  */
    virtual double derivative(double r) const;

    double sagitta(const Math::Vector2 & xy) const;
    void derivative(const Math::Vector2 & xy, Math::Vector2 & dxdy) const;

    // FIXME sample points
    /** Get number of available sample points. Samples points may be
      used by curve fitting algorithms and are choosen to avoid
      using interpolated points. return 0 when not applicable. */
    virtual unsigned int get_sample_count() const;
    /** Get a sample point on curve. (0,0) is not included. */
    virtual void get_sample(unsigned int index, double &r, double &z) const;

private:
    static double gsl_func_sagitta(double x, void *params);
    gsl_function gsl_func;
};

#endif // TROTATIONAL_H
