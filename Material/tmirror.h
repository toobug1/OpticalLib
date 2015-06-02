#ifndef TMIRROR_H
#define TMIRROR_H

#include "tdielectricbase.h"

class TMirror : public TDielectricBase
{
public:
    TMirror();

    /** @override */
    bool is_opaque(double wave) const;
    /** @override */
    bool is_reflecting(double wave) const;
    /** @override */
    double get_internal_transmittance(double wavelen, double thickness) const;
    /** @override */
    double get_refractive_index(double wavelen) const;
    /** @override */
    double get_extinction_coef(double wavelen) const;
};

extern TMirror mirror;

#endif // TMIRROR_H
