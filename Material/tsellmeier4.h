#ifndef TSELLMEIER4_H
#define TSELLMEIER4_H

#include "tdielectricbase.h"

class TSellmeier4 : public TDielectricBase
{
public:
    TSellmeier4();
    virtual ~TSellmeier4();

    TSellmeier4(double temp, const QString& name);

    TSellmeier4(double temp, const QString& name,
               const double* dispCoeff, int dispDispCoeffSize,
               const double* tempCoeff, int tempDispCoeffSize);

    enum{DispCoeffSize = 5};

    double get_refractive_index(double wavelen) const;

    int  setDispCoeff(const double* dispCoeff, int size);
    void setDispCoeff(double A, double B, double C, double D, double E);

    int getDispCoeff(double* dispCoeff, int size = DispCoeffSize) const;

private:
    double m_A;
    double m_B;
    double m_C;
    double m_D;
    double m_E;
};

#endif // TSELLMEIER4_H
