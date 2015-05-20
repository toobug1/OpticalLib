#ifndef THANDBOOK1_H
#define THANDBOOK1_H

#include "tdielectricbase.h"

class THandbook1 : public TDielectricBase
{
public:
    THandbook1();
    virtual ~THandbook1();
    THandbook1(double temp, const QString& name);

    THandbook1(double temp, const QString& name,
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

#endif // THANDBOOK1_H
