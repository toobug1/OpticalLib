#include "trotational.h"
#include <Math/Vector>

double TRotational::sagitta(const Math::Vector2 & xy) const
{
    return sagitta(xy.len());
}

TRotational::TRotational()
{
    gsl_func.function = gsl_func_sagitta;
    gsl_func.params = this;
}

void TRotational::normal(Math::Vector3 &normal, const Math::Vector3 &point) const
{
    const double r = sqrt(Math::square(point.x()) + Math::square(point.y()));

    if (r == 0)
        normal = Math::Vector3(0, 0, -1);
    else
    {
        const double p = derivative(r);

        normal = Math::Vector3(point.x() * p / r, point.y() * p / r, -1.0);
        normal.normalize(); // FIXME simplify ?
    }
}

void TRotational::derivative(const Math::Vector2 & xy, Math::Vector2 & dxdy) const
{
    const double r = xy.len();

    if (r == 0)
    {
        dxdy.x() = dxdy.y() = 0.0;
        return;
    }

    const double p = derivative(r);

    dxdy = xy * (p / r);
}

double TRotational::gsl_func_sagitta(double x, void *params)
{
    TRotational *c = static_cast<TRotational *>(params);

    return c->sagitta(x);
}

double TRotational::derivative(double r) const
{
    double result, abserr;

    gsl_deriv_central(&gsl_func, r, 1e-4, &result, &abserr);

    return result;
}

unsigned int TRotational::get_sample_count() const
{
    return 0;
}

void TRotational::get_sample(unsigned int index, double &r, double &z) const
{
}


