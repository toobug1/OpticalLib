#ifndef TSCHOTT_H
#define TSCHOTT_H

#include "tdielectricbase.h"

class TSchott : public TDielectricBase
{
public:
    TSchott();
    TSchott(double temp);
    ~TSchott();

    virtual double get_refractive_index(double wavelen, double temperature,
                                        double relativePressure) const;

private:
    double A0;
    double A1;
    double A2;
    double A3;
    double A4;
    double A5;
};

#endif // TSCHOTT_H
