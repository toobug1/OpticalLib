#ifndef TSELLMEIER1_H
#define TSELLMEIER1_H

#include "tdielectricbase.h"

class TSellmeier1 : public TDielectricBase
{
public:
    TSellmeier1();
    virtual ~TSellmeier1();

    TSellmeier1(double temp, const QString& name);

    TSellmeier1(double temp, const QString& name,
            const double* dispCoeff, int dispDispCoeffSize,
            const double* tempCoeff, int tempDispCoeffSize);

    enum{DispCoeffSize = 6};

    double get_refractive_index(double wavelen) const;

    int  setDispCoeff(const double* dispCoeff, int size);
    void setDispCoeff(double K1, double L1, double K2,
                      double L2, double K3, double L3);

    int getDispCoeff(double* dispCoeff, int size = DispCoeffSize) const;

private:
    double m_K1;
    double m_L1;
    double m_K2;
    double m_L2;
    double m_K3;
    double m_L3;
};

#endif // TSELLMEIER1_H
