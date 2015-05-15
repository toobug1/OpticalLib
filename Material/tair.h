#ifndef TAIR_H
#define TAIR_H

#include "tmaterialbase.h"

class TAir : public TMaterialBase
{
public:
    TAir();
    ~TAir();

    /** @override */
    bool is_opaque(double wavelen) const;
    /** @override */
    bool is_reflecting(double wavelen) const;

    /** @override */
    double get_internal_transmittance(double wavelen, double thickness = 1.0) const;
    /** @override */
    double get_refractive_index(double wavelen) const;
    /** @override */
    double get_extinction_coef(double wavelen) const;
};

#endif // TAIR_H
