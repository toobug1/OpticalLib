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

    return 1.0;
}

