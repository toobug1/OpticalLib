#include "tmaterialbase.h"

#include <math.h>

#include <gsl/gsl_spline.h>


static double square(double x)
{
    return x * x;
}

TMaterialBase::TMaterialBase(): m_temp(20.0)
{

}

TMaterialBase::~TMaterialBase()
{

}

double TMaterialBase::get_extinction_coef(double wavelen) const
{
    // Beer-Lambert law
//    return - (log(get_internal_transmittance(wavelen, 1.0)) * (wavelen * 1e-9f)) /
//            (4 * M_PI * 0.001/* 1 mm */);
    if (m_absorbMap.isEmpty())
    {
        return 0.0;
    }
    else if (m_absorbMap.contains(wavelen))
    {
        return m_absorbMap.value(wavelen);
    }
    else
    {
        return getInterpOfAbsorbCoeff(wavelen);
    }
}

double TMaterialBase::get_internal_transmittance(double wavelen,
                                                 double thickness) const
{
    // Beer-Lambert law

    double absorbCoeff = get_extinction_coef(wavelen);
    return pow(M_E, -absorbCoeff * thickness);
}


// default reflectance at normal incidence, valid for metal and dielectric material
double TMaterialBase::get_normal_reflectance(const TMaterialBase *from,
                                             double wavelen) const
{
    // McGraw Hill, Handbook of optics, vol1, 1995, 5-10 (47)

    double n0 = from->get_refractive_index(wavelen);
    double k12 = square(get_extinction_coef(wavelen));
    double n1 = get_refractive_index(wavelen);
    double res = (square(n0 - n1) + k12) / (square(n0 + n1) + k12);

    return res;
}

// default transmittance at normal incidence, valid for non absorbing material
double TMaterialBase::get_normal_transmittance(const TMaterialBase *from,
                                               double wavelen) const
{
    // McGraw Hill, Handbook of optics, vol1, 1995, 5-8 (23)

    double n0 = from->get_refractive_index(wavelen);
    double n1 = get_refractive_index(wavelen);

    return (4.0 * n0 * n1) / square(n0 + n1);
}


void TMaterialBase::set_temperature(double temp)
{
    m_temp = temp;
}

double TMaterialBase::get_temperature() const
{
    return m_temp;
}

void TMaterialBase::insertAbsorbedCoeffi(double wavelen, double trans, double thichness)
{
   double absorbCoeff = -(log(trans) / thichness);

   m_absorbMap.insert(wavelen, absorbCoeff);
}


double TMaterialBase::getInterpOfAbsorbCoeff(double wavelen) const
{
    int count = m_absorbMap.size();
    if (0 == count)
    {
        // no data means the material isn't absorb for any wavelength.

        return 0.0;
    }

    if (count < 2)
    {
        // if only contains one elements in map, directly return first.

        return m_absorbMap.first();
    }

    if (wavelen <= m_absorbMap.firstKey())
    {
        return m_absorbMap.first();
    }
    if (wavelen >= m_absorbMap.lastKey())
    {
        return m_absorbMap.last();
    }

    // otherwise, use GSL interpolation algorithm

    double x[count], y[count], rslt;
    int i = 0;
    QMap<double, double>::const_iterator iter = m_absorbMap.begin();
    while (iter != m_absorbMap.end())
    {
        x[i] = iter.key();
        y[i] = iter.value();
        ++i;
        ++iter;
    }

    gsl_interp_accel *acc = gsl_interp_accel_alloc();
    gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline, count);
    gsl_spline_init (spline, x, y, count);

    rslt = gsl_spline_eval (spline, wavelen, acc);

    gsl_spline_free(spline);
    gsl_interp_accel_free(acc);

    return rslt;
}

int TMaterialBase::removeAbsorbedCoeffi(double wavelen)
{
    return m_absorbMap.remove(wavelen);
}




