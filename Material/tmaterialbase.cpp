#include "tmaterialbase.h"

#include <math.h>

#include <gsl/gsl_interp.h>


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
    // FIXME check this formula
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
        // to get a linear interpolation
        return getLinearInterpolation(wavelen);
    }
}

double TMaterialBase::get_internal_transmittance(double wavelen,
                                                 double thickness) const
{
    // Beer-Lambert law

    // FIXME simplify and check
    double absorbCoeff = get_extinction_coef(wavelen);

    return pow(M_E, -absorbCoeff * thickness);
}

// compute internal transmittance from extinction coefficient
double TMaterialBase::get_internal_transmittance(double wavelen) const
{
    return get_internal_transmittance(wavelen, 1.0);
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


double TMaterialBase::getLinearInterpolation(double wavelen) const
{
    int count = m_absorbMap.size();
    if (0 == count)
    {
        return 0.0;
    }

    if (count < 2)
    {
        // if only contains one elements in map, directly return first.
        return m_absorbMap.first();
    }

    // otherwise, use GSL linear interpolation algorithm

    // todo



    return 0.0;
}

int TMaterialBase::removeAbsorbedCoeffi(double wavelen)
{
    return m_absorbMap.remove(wavelen);
}




