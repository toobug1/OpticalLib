#ifndef THANDBOOK2_H
#define THANDBOOK2_H

#include "tdielectricbase.h"

class THandbook2 : public TDielectricBase
{
public:
    THandbook2();
    virtual ~THandbook2();
    THandbook2(double temp, const QString& name);

    THandbook2(double temp, const QString& name,
               const double* dispCoeff, int dispDispCoeffSize,
               const double* tempCoeff, int tempDispCoeffSize);

    enum{DispCoeffSize = 4};

    double get_refractive_index(double wavelen) const;

    int  setDispCoeff(const double* dispCoeff, int size);
    void setDispCoeff(double A, double B, double C, double D);

    int getDispCoeff(double* dispCoeff, int size = DispCoeffSize) const;

private:
    double m_A;
    double m_B;
    double m_C;
    double m_D;
};

#endif // THANDBOOK2_H
