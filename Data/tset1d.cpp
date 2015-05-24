#include "tset1d.h"

#include <assert.h>

TSet1d::TSet1d()
    : TSet()
{
}

TSet1d::~TSet1d()
{
}

unsigned int TSet1d::get_dimensions() const
{
    return 1;
}

unsigned int TSet1d::get_count(unsigned int dimension) const
{
    assert(dimension == 0);
    return get_count();
}

double TSet1d::get_x_value(unsigned int x, unsigned int dimension) const
{
    assert(dimension == 0);
    return get_x_value(x);
}

double TSet1d::get_y_value(const unsigned int x[]) const
{
    return get_y_value(x[0]);
}

Math::range_t TSet1d::get_x_range(unsigned int dimension) const
{
    assert(dimension == 0);
    return get_x_range();
}

double TSet1d::interpolate(const double x[]) const
{
    return interpolate(x[0]);
}

double TSet1d::interpolate(const double x[], unsigned int deriv, unsigned int dimension) const
{
    assert(dimension == 0);
    return interpolate(x[0], deriv);
}

