#ifndef TDIELECTRIC_H
#define TDIELECTRIC_H

#include "tmaterialbase.h"


class TDielectric : public TMaterialBase
{
public:
    TDielectric();
    TDielectric(double temp);
    ~TDielectric();

    virtual bool is_opaque(double wavelen) const = 0;
    virtual bool is_reflecting(double wavelen) const = 0;

    virtual double get_refractive_index(double wavelen) const = 0;
    virtual double get_refractive_index(double wavelen, double temperature,
                                double relativePressure) const = 0;
};

#endif // TDIELECTRIC_H
