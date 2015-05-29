#ifndef TINTERPOLATE1D_H
#define TINTERPOLATE1D_H

/** @internal */

#include "common.hh"

enum cubic_2nd_deriv_init_e
{
    Cubic2ndDerivQuadratic,
    Cubic2ndDerivFirst,
    Cubic2ndDerivSecond,
};

/*
   @short Common iterpolation code for 1d data sets
   @module {Core}
   @internal
 */

using namespace _Goptical::Data;

template <class X> class TInterpolate1d : public X
{

public:
    TInterpolate1d()  {}
    ~TInterpolate1d() {}

    inline double interpolate(const double x) const;
    inline double interpolate(const double x, unsigned int deriv) const;

    void set_interpolation(Interpolation i) {this->_interpolation = i;}

private:
    /** quadratic and cubic polynomial coefficients */
    struct poly_s
    {
        double a, b, c, d;
    };

    /** set polynomial coefficients of line through 2 points */
    static void set_linear_poly(struct poly_s &p,
                                double p1x, double p1y,
                                double p2x, double p2y);

    /** set polynomial coefficients of line through 1 points with given derivative */
    static void set_linear_poly(struct poly_s &p,
                                double p1x, double p1y,
                                double d1);

    /** set polynomial coefficients of parabola through 3 points */
    static void set_quadratic_poly(struct poly_s &p,
                                   double p1x, double p1y,
                                   double p2x, double p2y,
                                   double p3x, double p3y);

    /** set polynomial coefficients of parabola through 1 points with
      given first and second derivatives */
    static void set_quadratic_poly(struct poly_s &p,
                                   double px, double py,
                                   double d, double dd);

    /** set polynomial coefficients of cubic through 2 points with
      given first derivatives */
    static void set_cubic_poly(struct poly_s &p,
                               double p1x, double p1y,
                               double p2x, double p2y,
                               double d1, double d2);

    /** set polynomial coefficients of cubic through 2 points with
      given second derivatives */
    static void set_cubic_poly2(struct poly_s &p,
                                double p1x, double p1y,
                                double p2x, double p2y,
                                double dd1, double dd2);

    /** compute cubic sencond derivatives using the tridiagonal algorithm */
    template <cubic_2nd_deriv_init_e de>
    void compute_cubic_2nd_deriv(unsigned int n, double dd[],
                                 double d0, double dn) const;

    double update_nearest(unsigned int d, double x) const;
    double interpolate_nearest(unsigned int d, double x) const;

    double update_linear(unsigned int d, double x) const;
    double interpolate_linear(unsigned int d, double x) const;

    double update_quadratic(unsigned int d, double x) const;
    double interpolate_quadratic(unsigned int d, double x) const;

    double update_cubic(unsigned int d, double x) const;
    double update_cubic2(unsigned int d, double x) const;
    double update_cubic_deriv(unsigned int d, double x) const;
    double update_cubic2_deriv(unsigned int d, double x) const;
    double update_cubic_simple(unsigned int d, double x) const;
    double update_cubic_deriv_init(unsigned int d, double x) const;
    double update_cubic2_deriv_init(unsigned int d, double x) const;
    double interpolate_cubic(unsigned int d, double x) const;

//    void invalidate(){X::invalidate();}

    double (TInterpolate1d::*_update)(unsigned int d, double x) const;
    double (TInterpolate1d::*_interpolate)(unsigned int d, double x) const;

    std::vector<struct poly_s>        _poly;
};

template <class X>
double TInterpolate1d<X>::interpolate(double x) const
{
    return (this->*_interpolate)(0, x);
}

template <class X>
double TInterpolate1d<X>::interpolate(double x, unsigned int d) const
{
    return (this->*_interpolate)(d, x);
}

#endif // TINTERPOLATE1D_H
