#ifndef TINTERPOLATE1D_H
#define TINTERPOLATE1D_H

/** @internal */

#include "common.hh"
#include "Error"
#include "Math/Vector"

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
using namespace _Goptical;

template <class X> class TInterpolate1d : public X
{

public:
    TInterpolate1d();
    ~TInterpolate1d();

    inline double interpolate(const double x) const;
    inline double interpolate(const double x, unsigned int deriv) const;

    void set_interpolation(Interpolation i);

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

    void invalidate();

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


template <class X>
TInterpolate1d<X>::TInterpolate1d()
    : _update(&TInterpolate1d::update_linear),
      _interpolate(_update),
      _poly()
{
}

template <class X>
TInterpolate1d<X>::~TInterpolate1d()
{
}

template <class X>
template <cubic_2nd_deriv_init_e de>
void TInterpolate1d<X>::compute_cubic_2nd_deriv(unsigned int n, double dd[],
                                                double d0, double dn) const
{
    double eq[n][3];

    // first and last tridiag system equations
    switch (de)
    {
    case Cubic2ndDerivQuadratic:
        dd[0  ] = dd[n-1] = 0.0;
        eq[0  ][1] = eq[n-1][1] = 1.0;
        eq[1  ][0] = eq[n-2][2] = -1.0;
        break;

    case Cubic2ndDerivFirst: {
        // first derivative is prescribed for first and last point
        double x0 = X::get_x_interval(0);
        double xn = X::get_x_interval(n - 2);

        dd[0  ] = (X::get_y_value(1) - X::get_y_value(0)) / x0 - d0;
        dd[n-1] = dn - (X::get_y_value(n - 1) - X::get_y_value(n - 2)) / xn;
        eq[0  ][1] = x0 / 3.0;
        eq[1  ][0] = x0 / 6.0;
        eq[n-2][2] = xn / 6.0;
        eq[n-1][1] = xn / 3.0;
        break;
    }

    case Cubic2ndDerivSecond:
        // second derivative is prescribed for first and last point
        dd[0  ] = d0;
        dd[n-1] = dn;
        eq[1  ][0] = eq[n-2][2] = 0.0;
        eq[0  ][1] = eq[n-1][1] = 1.0;
        break;
    }

    int i;

    // middle tridiag system equations
    for (i = 1; i < (int)n - 1; i++)
    {
        eq[i-1][2] = X::get_x_interval(i - 1) / 6.0;
        eq[i  ][1] = X::get_x_interval(i - 1, i + 1) / 3.0;
        eq[i+1][0] = X::get_x_interval(i) / 6.0;
        dd[i] = ( X::get_y_value(i + 1) - X::get_y_value(i) ) / X::get_x_interval(i)
                - ( X::get_y_value(i) - X::get_y_value(i - 1) ) / X::get_x_interval(i - 1);
    }

    // solve tridiag system
    // forward substitution
    for (i = 1; i < (int)n; i++)
    {
        double f = eq[i-1][2] / eq[i-1][1];
        eq[i][1] -= f * eq[i][0];
        dd[i]    -= f * dd[i-1];
    }

    // backward substitution
    double k = 0;
    for (i = n - 1; i >= 0; i--)
    {
        double ddi = (dd[i] - k) / eq[i][1];
        dd[i] = ddi;
        k = eq[i][0] * ddi;
    }
}

template <class X>
inline void TInterpolate1d<X>::set_linear_poly(struct poly_s &p,
                                               double p1x, double p1y,
                                               double p2x, double p2y)
{
    p.a = 0.0;
    p.b = 0.0;
    p.c = (p1y - p2y)/(p1x - p2x);
    p.d = (p2x * p1y - p1x * p2y)/(p2x - p1x);
}

template <class X>
inline void TInterpolate1d<X>::set_linear_poly(struct poly_s &p,
                                               double p1x, double p1y,
                                               double d1)
{
    p.a = 0.0;
    p.b = 0.0;
    p.c = d1;
    p.d = p1y - d1 * p1x;
}

template <class X>
inline void TInterpolate1d<X>::set_quadratic_poly(struct poly_s &p,
                                                  double p1x, double p1y,
                                                  double p2x, double p2y,
                                                  double p3x, double p3y)
{
    double n = ((p2x - p1x) * (p3x - p1x) * (p3x - p2x));

    p.a = 0.0;

    p.b = (p3y * (p2x - p1x) +
           p2y * (p1x - p3x) +
           p1y * (p3x - p2x)) / n;

    double p1x2 = Math::square(p1x);
    double p2x2 = Math::square(p2x);
    double p3x2 = Math::square(p3x);

    p.c = (p3y * (p1x2 - p2x2) +
           p2y * (p3x2 - p1x2) +
           p1y * (p2x2 - p3x2)) / n;

    p.d = (p3y * (p1x * p2x2 - p2x * p1x2) +
           p2y * (p3x * p1x2 - p1x * p3x2) +
           p1y * (p2x * p3x2 - p3x * p2x2)) / n;
}

template <class X>
inline void TInterpolate1d<X>::set_cubic_poly(struct poly_s &p,
                                              double p1x, double p1y,
                                              double p2x, double p2y,
                                              double d1, double d2)
{
    /*
    a=-(-2*y2+2*y1+(d2+d1)*x2+(-d2-d1)*x1)/(-x2^3+3*x1*x2^2-3*x1^2*x2+x1^3),
    b=(-3*x2*y2+x1*((d2-d1)*x2-3*y2)+(3*x2+3*x1)*y1+(d2+2*d1)*x2^2+(-2*d2-d1)*x1^2)/(-x2^3+3*x1*x2^2-3*x1^2*x2+x1^3),
    c=-(x1*((2*d2+d1)*x2^2-6*x2*y2)+6*x1*x2*y1+d1*x2^3+(-d2-2*d1)*x1^2*x2-d2*x1^3)/(-x2^3+3*x1*x2^2-3*x1^2*x2+x1^3),
    d=(x1^2*((d2-d1)*x2^2-3*x2*y2)+x1^3*(y2-d2*x2)+(3*x1*x2^2-x2^3)*y1+d1*x1*x2^3)/(-x2^3+3*x1*x2^2-3*x1^2*x2+x1^3)
  */
    double x1 = p1x;
    double x2 = p2x;
    double y1 = p1y;
    double y2 = p2y;

    // FIXME simplify

    p.a = - ( 2. * y1 - 2. * y2 + ( d2 + d1)  * x2 - ( d2 + d1)  * x1)  / ( 3. * x1 * x2*x2 - x2*x2*x2 - 3. * x1*x1 * x2 + x1*x1*x1);

    p.b = ( x1 * ( ( d2 - d1)  * x2 - 3. * y2)  - 3. * x2 * y2 + ( 3. * x2 + 3. * x1)  * y1 + ( d2 + 2. * d1)  * x2*x2 - ( 2. * d2 + d1)  * x1*x1)  / ( 3. * x1 * x2*x2 - x2*x2*x2 - 3. * x1*x1 * x2 + x1*x1*x1);

    p.c = - ( x1 * ( ( 2. * d2 + d1)  * x2*x2 - 6. * x2 * y2)  + 6. * x1 * x2 * y1 + d1 * x2*x2*x2 - ( d2 + 2. * d1)  * x1*x1 * x2 - d2 * x1*x1*x1)  / ( 3. * x1 * x2*x2 - x2*x2*x2 - 3. * x1*x1 * x2 + x1*x1*x1);

    p.d = ( x1*x1 * ( ( d2 - d1)  * x2*x2 - 3. * x2 * y2)  + x1*x1*x1 * ( y2 - d2 * x2)  + ( 3. * x1 * x2*x2 - x2*x2*x2)  * y1 + d1 * x1 * x2*x2*x2)  / ( 3. * x1 * x2*x2 - x2*x2*x2 - 3. * x1*x1 * x2 + x1*x1*x1);
}

template <class X>
inline void TInterpolate1d<X>::set_cubic_poly2(struct poly_s &p,
                                               double p1x, double p1y,
                                               double p2x, double p2y,
                                               double dd1, double dd2)
{
    /*
    a=(dd1-dd2)/(6*x1-6*x2);
    b=(dd2*x1-dd1*x2)/(2*x1-2*x2);
    c=(-6*y2+6*y1+(dd2+2*dd1)*x2^2+(2*dd1-2*dd2)*x1*x2+(-2*dd2-dd1)*x1^2)/(6*x1-6*x2);
    d=-(x1*((dd2+2*dd1)*x2^2-6*y2)+6*x2*y1+(-2*dd2-dd1)*x1^2*x2)/(6*x1-6*x2);
  */

    // FIXME simplify

    p.a = ( dd1 - dd2)  / ( 6. * p1x - 6. * p2x);

    p.b = ( dd2 * p1x - dd1 * p2x)  / ( 2. * p1x - 2. * p2x);

    p.c = ( 6. * p1y - 6. * p2y + ( dd2 + 2. * dd1)  * p2x*p2x
            + ( 2. * dd1 - 2. * dd2)  * p1x * p2x - ( 2. * dd2 + dd1)  * p1x*p1x)  / ( 6. * p1x - 6. * p2x);

    p.d =  - ( p1x * ( ( dd2 + 2. * dd1)  * p2x*p2x - 6. * p2y)
               + 6. * p2x * p1y - ( 2. * dd2 + dd1)  * p1x*p1x * p2x)  / ( 6. * p1x - 6. * p2x);
}


template <class X>
inline void TInterpolate1d<X>::set_quadratic_poly(struct poly_s &p,
                                                  double px, double py,
                                                  double d, double dd)
{
    p.a = 0;
    p.b = dd / 2.0;
    p.c = -px*dd + d;
    p.d = 0.5*px*px*dd - px*d + py;
}

template <class X>
void TInterpolate1d<X>::set_interpolation(Interpolation i)
{
    switch (i)
    {
    case Nearest:
        _update = &TInterpolate1d::update_nearest;
        _poly.clear();
        break;

    case Linear:
        _update = &TInterpolate1d::update_linear;
        _poly.clear();
        break;

    case Quadratic:
        _update = &TInterpolate1d::update_quadratic;
        break;

    case CubicSimple:
        _update = &TInterpolate1d::update_cubic_simple;
        break;

    case CubicDeriv:
        _update = &TInterpolate1d::update_cubic_deriv;
        break;

    case Cubic2Deriv:
        _update = &TInterpolate1d::update_cubic2_deriv;
        break;

    case CubicDerivInit:
        _update = &TInterpolate1d::update_cubic_deriv_init;
        break;

    case Cubic2DerivInit:
        _update = &TInterpolate1d::update_cubic2_deriv_init;
        break;

    case Cubic:
        _update = &TInterpolate1d::update_cubic;
        break;

    case Cubic2:
        _update = &TInterpolate1d::update_cubic2;
        break;

    default:
        throw Error("invalid interpolation selected");
    }

    X::_interpolation = i;
    _interpolate = _update;
}

template <class X>
double TInterpolate1d<X>::interpolate_nearest(unsigned int d, double x) const
{
    switch (d)
    {
    case (0):
        return X::get_y_value(X::get_nearest(x));

    default:
        return 0.0;
    }
}

template <class X>
double TInterpolate1d<X>::update_nearest(unsigned int d, double x) const
{
    TInterpolate1d * this_ = const_cast<TInterpolate1d *>(this);

    if (X::get_count() == 0)
        throw Error("data set contains no data");

    this_->_interpolate = &TInterpolate1d::interpolate_nearest;

    return interpolate_nearest(d, x);
}

template <class X>
double TInterpolate1d<X>::interpolate_linear(unsigned int d, double x) const
{
    unsigned int di = X::get_interval(x);

    if (!di)
        di++;
    else if (di == X::get_count())
        di--;

    switch (d)
    {
    case (0): {
        double mu = (x - X::get_x_value(di - 1)) / (X::get_x_interval(di - 1));

        return X::get_y_value(di - 1) * (1.0 - mu) + X::get_y_value(di) * mu;
    }

    case (1): {
        return (X::get_y_value(di) - X::get_y_value(di - 1)) / (X::get_x_interval(di - 1));
    }

    default: {
        return 0.0;
    }
    }
}

template <class X>
double TInterpolate1d<X>::update_linear(unsigned int d, double x) const
{
    TInterpolate1d * this_ = const_cast<TInterpolate1d *>(this);

    if (X::get_count() < 2)
        throw Error("data set doesn't contains enough data");

    this_->_interpolate = &TInterpolate1d::interpolate_linear;

    return interpolate_linear(d, x);
}

template <class X>
double TInterpolate1d<X>::interpolate_quadratic(unsigned int d, double x) const
{
    const struct poly_s &p = _poly[X::get_nearest(x)];

    switch (d)
    {
    case (0):
        return x * (p.b * x + p.c) + p.d;

    case (1):
        return 2.0 * p.b * x + p.c;

    case (2):
        return 2.0 * p.b;

    default:
        return 0.0;
    }
}

template <class X>
double TInterpolate1d<X>::update_quadratic(unsigned int d, double x) const
{
    TInterpolate1d * this_ = const_cast<TInterpolate1d *>(this);
    std::vector<struct poly_s> & poly = this_->_poly;

    if (X::get_count() < 3)
        throw Error("data set doesn't contains enough data");

    poly.resize(X::get_count());

    set_linear_poly(poly[0], X::get_x_value(0), X::get_y_value(0),
            X::get_x_value(1), X::get_y_value(1));

    unsigned int i;

    for (i = 1; i < X::get_count() - 1; i++)
    {
        double p1x = (X::get_x_value(i - 1) + X::get_x_value(i)) / 2.0;
        double p1y = (X::get_y_value(i - 1) + X::get_y_value(i)) / 2.0;

        double p3x = (X::get_x_value(i) + X::get_x_value(i + 1)) / 2.0;
        double p3y = (X::get_y_value(i) + X::get_y_value(i + 1)) / 2.0;

        set_quadratic_poly(poly[i],
                           p1x, p1y,
                           X::get_x_value(i), X::get_y_value(i),
                           p3x, p3y);
    }

    set_linear_poly(poly[i],
                    X::get_x_value(i - 1), X::get_y_value(i - 1),
                    X::get_x_value(i), X::get_y_value(i));

    this_->_interpolate = &TInterpolate1d::interpolate_quadratic;

    return interpolate_quadratic(d, x);
}

template <class X>
double TInterpolate1d<X>::interpolate_cubic(unsigned int d, double x) const
{
    const struct poly_s &p = _poly[X::get_interval(x)];

    switch (d)
    {
    case (0):
        return ((p.a * x + p.b) * x + p.c) * x + p.d;

    case (1):
        return (3.0 * p.a * x + 2.0 * p.b) * x + p.c;

    case (2):
        return 6.0 * p.a * x + 2.0 * p.b;

    case (3):
        return 6.0 * p.a;

    default:
        return 0.0;
    }
}

template <class X>
double TInterpolate1d<X>::update_cubic_simple(unsigned int d, double x) const
{
    TInterpolate1d * this_ = const_cast<TInterpolate1d *>(this);
    std::vector<struct poly_s> & poly = this_->_poly;

    unsigned int n = X::get_count();

    if (n < 4)
        throw Error("data set doesn't contains enough data");

    poly.resize(n + 1);

    Math::Vector2 vm1(X::get_x_value(0), X::get_y_value(0));
    Math::Vector2 vm2(vm1);
    Math::Vector2 v(X::get_x_value(1), X::get_y_value(1));
    Math::Vector2 vp1(X::get_x_value(2), X::get_y_value(2));

    double d1 = (v.y() - vm1.y())   / (v.x() - vm1.x());
    double d2 = (vp1.y() - vm1.y()) / (vp1.x() - vm1.x());

    // extrapolation
    set_linear_poly(poly[0], vm1.x(), vm1.y(), d1);

    // first segment
    set_cubic_poly(poly[1], vm1.x(), vm1.y(), v.x(), v.y(), d1, d2);

    for (unsigned int i = 2; i < n - 1; i++)
    {
        vm2 = vm1;
        vm1 = v;
        v = vp1;
        vp1 = Math::Vector2(X::get_x_value(i+1), X::get_y_value(i+1));

        d1 = d2;
        d2 = (vp1.y() - vm1.y()) / (vp1.x() - vm1.x());

        set_cubic_poly(poly[i], vm1.x(), vm1.y(), v.x(), v.y(), d1, d2);
    }

    d1 = d2;
    d2 = (vp1.y() - v.y()) / (vp1.x() - v.x());

    // last segment
    set_cubic_poly(poly[n - 1], v.x(), v.y(), vp1.x(), vp1.y(), d1, d2);

    // extrapolation
    set_linear_poly(poly[n], vp1.x(), vp1.y(), d2);

    this_->_interpolate = &TInterpolate1d::interpolate_cubic;

    return interpolate_cubic(d, x);
}


template <class X>
double TInterpolate1d<X>::update_cubic(unsigned int d, double x) const
{
    TInterpolate1d * this_ = const_cast<TInterpolate1d *>(this);
    std::vector<struct poly_s> & poly = this_->_poly;

    unsigned int n = X::get_count();

    if (n < 4)
        throw Error("data set doesn't contains enough data");

    poly.resize(n + 1);

    double d0 = (X::get_y_value(1) - X::get_y_value(0)) / X::get_x_interval(0);
    double dn = (X::get_y_value(n - 1) - X::get_y_value(n - 2)) / X::get_x_interval(n - 2);
    double dd[n];

    compute_cubic_2nd_deriv<Cubic2ndDerivFirst>(n, dd, d0, dn);

    set_linear_poly(poly[0], X::get_x_value(0), X::get_y_value(0), d0);

    for (unsigned int i = 1; i < n; i++)
        set_cubic_poly2(poly[i],
                        X::get_x_value(i - 1), X::get_y_value(i - 1),
                        X::get_x_value(i), X::get_y_value(i),
                        dd[i - 1], dd[i]);

    set_linear_poly(poly[n], X::get_x_value(n-1), X::get_y_value(n-1), dn);

    this_->_interpolate = &TInterpolate1d::interpolate_cubic;

    return interpolate_cubic(d, x);

}

template <class X>
double TInterpolate1d<X>::update_cubic2(unsigned int d, double x) const
{
    TInterpolate1d * this_ = const_cast<TInterpolate1d *>(this);
    std::vector<struct poly_s> & poly = this_->_poly;

    unsigned int n = X::get_count();

    if (n < 4)
        throw Error("data set doesn't contains enough data");

    poly.resize(n + 1);

    double d0 = (X::get_y_value(1) - X::get_y_value(0)) / X::get_x_interval(0);
    double dn = (X::get_y_value(n - 1) - X::get_y_value(n - 2)) / X::get_x_interval(n - 2);
    double dd[n];

    compute_cubic_2nd_deriv<Cubic2ndDerivFirst>(n, dd, d0, dn);

    set_quadratic_poly(poly[0],
            X::get_x_value(0), X::get_y_value(0),
            d0, dd[0]);

    for (unsigned int i = 1; i < n; i++)
        set_cubic_poly2(poly[i],
                        X::get_x_value(i - 1), X::get_y_value(i - 1),
                        X::get_x_value(i), X::get_y_value(i),
                        dd[i - 1], dd[i]);

    set_quadratic_poly(poly[n],
                       X::get_x_value(n-1), X::get_y_value(n-1),
                       dn, dd[n-1]);

    this_->_interpolate = &TInterpolate1d::interpolate_cubic;

    return interpolate_cubic(d, x);

}

template <class X>
double TInterpolate1d<X>::update_cubic_deriv_init(unsigned int d, double x) const
{
    TInterpolate1d * this_ = const_cast<TInterpolate1d *>(this);
    std::vector<struct poly_s> & poly = this_->_poly;

    unsigned int n = X::get_count();

    if (n < 4)
        throw Error("data set doesn't contains enough data");

    poly.resize(n + 1);

    double dd[n];
    double d0 = X::get_d_value(0);
    double dn = X::get_d_value(n - 1);

    compute_cubic_2nd_deriv<Cubic2ndDerivFirst>(X::get_count(), dd, d0, dn);

    set_linear_poly(poly[0], X::get_x_value(0), X::get_y_value(0), d0);

    for (unsigned int i = 1; i < n; i++)
        set_cubic_poly2(poly[i],
                        X::get_x_value(i - 1), X::get_y_value(i - 1),
                        X::get_x_value(i), X::get_y_value(i),
                        dd[i - 1], dd[i]);

    set_linear_poly(poly[n], X::get_x_value(n - 1), X::get_y_value(n - 1), dn);

    this_->_interpolate = &TInterpolate1d::interpolate_cubic;

    return interpolate_cubic(d, x);
}

template <class X>
double TInterpolate1d<X>::update_cubic2_deriv_init(unsigned int d, double x) const
{
    TInterpolate1d * this_ = const_cast<TInterpolate1d *>(this);
    std::vector<struct poly_s> & poly = this_->_poly;

    unsigned int n = X::get_count();

    if (n < 4)
        throw Error("data set doesn't contains enough data");

    poly.resize(n + 1);

    double dd[n];
    double d0 = X::get_d_value(0);
    double dn = X::get_d_value(n - 1);

    compute_cubic_2nd_deriv<Cubic2ndDerivFirst>(X::get_count(), dd, d0, dn);

    set_quadratic_poly(poly[0],
            X::get_x_value(0), X::get_y_value(0),
            d0, dd[0]);

    for (unsigned int i = 1; i < n; i++)
        set_cubic_poly2(poly[i],
                        X::get_x_value(i - 1), X::get_y_value(i - 1),
                        X::get_x_value(i), X::get_y_value(i),
                        dd[i - 1], dd[i]);

    set_quadratic_poly(poly[n],
                       X::get_x_value(n-1), X::get_y_value(n-1),
                       dn, dd[n-1]);

    this_->_interpolate = &TInterpolate1d::interpolate_cubic;

    return interpolate_cubic(d, x);
}

template <class X>
double TInterpolate1d<X>::update_cubic2_deriv(unsigned int d, double x) const
{
    TInterpolate1d * this_ = const_cast<TInterpolate1d *>(this);
    std::vector<struct poly_s> & poly = this_->_poly;

    unsigned int n = X::get_count();

    if (n < 4)
        throw Error("data set doesn't contains enough data");

    poly.resize(n + 1);

    double dd0 = (X::get_d_value(1) - X::get_d_value(0)) / X::get_x_interval(0);

    set_quadratic_poly(poly[0],
            X::get_x_value(0), X::get_y_value(0),
            X::get_d_value(0),dd0 );

    for (unsigned int i = 1; i < n; i++)
        set_cubic_poly(poly[i],
                       X::get_x_value(i - 1), X::get_y_value(i - 1),
                       X::get_x_value(i), X::get_y_value(i),
                       X::get_d_value(i - 1), X::get_d_value(i)
                       );

    double ddn = (X::get_d_value(n - 1) - X::get_d_value(n - 2)) / X::get_x_interval(n - 2);

    set_quadratic_poly(poly[n],
                       X::get_x_value(n-1), X::get_y_value(n-1),
                       X::get_d_value(n-1), ddn);

    this_->_interpolate = &TInterpolate1d::interpolate_cubic;

    return interpolate_cubic(d, x);
}

template <class X>
double TInterpolate1d<X>::update_cubic_deriv(unsigned int d, double x) const
{
    TInterpolate1d * this_ = const_cast<TInterpolate1d *>(this);
    std::vector<struct poly_s> & poly = this_->_poly;

    unsigned int n = X::get_count();

    if (n < 4)
        throw Error("data set doesn't contains enough data");

    poly.resize(n + 1);

    set_linear_poly(poly[0], X::get_x_value(0), X::get_y_value(0), X::get_d_value(0));

    for (unsigned int i = 1; i < n; i++)
        set_cubic_poly(poly[i],
                       X::get_x_value(i - 1), X::get_y_value(i - 1),
                       X::get_x_value(i), X::get_y_value(i),
                       X::get_d_value(i - 1), X::get_d_value(i)
                       );

    set_linear_poly(poly[n], X::get_x_value(n - 1), X::get_y_value(n - 1), X::get_d_value(n - 1));

    this_->_interpolate = &TInterpolate1d::interpolate_cubic;

    return interpolate_cubic(d, x);
}

template <class X>
void TInterpolate1d<X>::invalidate()
{
    _interpolate = _update;
}

#endif // TINTERPOLATE1D_H
