#include "tair.h"

TAir::TAir()
{

}

TAir::~TAir()
{

}

bool TAir::is_opaque(double wavelen) const
{
    return false;
}

bool TAir::is_reflecting(double wavelen) const
{
    return false;
}

double TAir::get_internal_transmittance(double wavelen, double thickness) const
{
    // there may be some formula
    // FIXME
    return 1.0;
}

double TAir::get_extinction_coef(double wavelen) const
{
    // there may be some formula
    // FIXME
    return 0.0;
}

double TAir::get_refractive_index(double wavelen) const
{

}

double TAir::get_refractive_index(double wavelen, double temperature,
                            double relativePressure) const
{

}

