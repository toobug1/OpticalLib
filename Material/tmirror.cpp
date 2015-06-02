#include "tmirror.h"

TMirror mirror;

TMirror::TMirror()
{
    m_name = "MIRROR";
}

bool TMirror::is_opaque(double wave) const
{
    return true;
}

bool TMirror::is_reflecting(double wave) const
{
    return true;
}

double TMirror::get_internal_transmittance(double wavelen, double thickness) const
{
    return 0.0;
}

double TMirror::get_refractive_index(double wavelen) const
{
    return 1.0;
}

double TMirror::get_extinction_coef(double wavelen) const
{
    return 99999.0;
}


