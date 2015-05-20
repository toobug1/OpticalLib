#ifndef TSELLMEIER5_H
#define TSELLMEIER5_H

#include "tdielectricbase.h"

class TSellmeier5 : public TDielectricBase
{
public:
    TSellmeier5();
    virtual ~TSellmeier5();

    TSellmeier5(double temp, const QString& name);

    TSellmeier5(double temp, const QString& name,
            const double* dispCoeff, int dispDispCoeffSize,
            const double* tempCoeff, int tempDispCoeffSize);

    enum{DispCoeffSize = 10};

    double get_refractive_index(double wavelen) const;

    int  setDispCoeff(const double* dispCoeff, int size);
    void setDispCoeff(double K1, double L1, double K2, double L2,
                      double K3, double L3, double K4, double L4,
                      double K5, double L5);

    int getDispCoeff(double* dispCoeff, int size = DispCoeffSize) const;

private:
    double m_K1;
    double m_L1;
    double m_K2;
    double m_L2;
    double m_K3;
    double m_L3;
    double m_K4;
    double m_L4;
    double m_K5;
    double m_L5;
};

#endif // TSELLMEIER5_H
