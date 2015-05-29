#include "tspline.h"


TSpline::TSpline()
    : _data()
{
    _data.set_interpolation(Data::Cubic);
}

TSpline::~TSpline()
{
}

void TSpline::fit(const TRotational &c, double radius, unsigned int points)
{
    double step = radius / points;
    _data.clear();

    for (double x = 0; x < radius + step / 2; x += step)
        _data.add_data(x, c.sagitta(x));
}

const TDiscreteSet & TSpline::get_data() const
{
    return _data;
}

TDiscreteSet & TSpline::get_data()
{
    return _data;
}

double TSpline::sagitta(double r) const
{
    return _data.interpolate(r);
}

double TSpline::derivative(double r) const
{
    return _data.interpolate(r, 1);
}




