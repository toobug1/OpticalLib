#ifndef TSELLMEIER3_H
#define TSELLMEIER3_H

#include "tdielectricbase.h"

class TSellmeier3 : public TDielectricBase
{
public:
    TSellmeier3();
    virtual ~TSellmeier3();

    TSellmeier3(double temp, const QString& name);

    TSellmeier3(double temp, const QString& name,
            const double* dispCoeff, int dispDispCoeffSize,
            const double* tempCoeff, int tempDispCoeffSize);

    enum{DispCoeffSize = 8};

    double get_refractive_index(double wavelen) const;

    int  setDispCoeff(const double* dispCoeff, int size);
    void setDispCoeff(double K1, double L1, double K2,
                      double L2, double K3, double L3,
                      double K4, double L4);

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
};

#endif // TSELLMEIER3_H
