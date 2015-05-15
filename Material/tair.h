#ifndef TAIR_H
#define TAIR_H

#include "tmaterialbase.h"

class TAir : public TMaterialBase
{
public:
    TAir();
    ~TAir();

    bool is_opaque(double wavelen) const;

    bool is_reflecting(double wavelen) const;

    double get_internal_transmittance(double wavelen, double thickness = 1.0) const;

    double get_refractive_index(double wavelen) const; // 20deg one atmosphere
    double get_refractive_index(double wavelen, double temperature,
                                double relativePressure) const;

    double get_extinction_coef(double wavelen) const;
};

#endif // TAIR_H