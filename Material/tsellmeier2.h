#ifndef TSELLMEIER2_H
#define TSELLMEIER2_H

#include "tdielectricbase.h"

class TSellmeier2 : public TDielectricBase
{
public:
    TSellmeier2();
    virtual ~TSellmeier2();

    TSellmeier2(double temp, const QString& name);

    TSellmeier2(double temp, const QString& name,
                const double* dispCoeff, int dispDispCoeffSize,
                const double* tempCoeff, int tempDispCoeffSize);

    enum{DispCoeffSize = 5};

    double get_refractive_index(double wavelen) const;

    int  setDispCoeff(const double* dispCoeff, int size);
    void setDispCoeff(double A, double B1, double B2,
                      double Lamd1, double Lamd2);

    int getDispCoeff(double* dispCoeff, int size = DispCoeffSize) const;

private:
    double m_A;
    double m_B1;
    double m_B2;
    double m_Lamd1;
    double m_Lamd2;
};

#endif // TSELLMEIER2_H
