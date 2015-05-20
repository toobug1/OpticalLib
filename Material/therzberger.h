#ifndef THERZBERGER_H
#define THERZBERGER_H

#include "tdielectricbase.h"

class THerzberger : public TDielectricBase
{
public:
    THerzberger();
    virtual ~THerzberger();

    THerzberger(double temp, const QString& name);

    THerzberger(double temp, const QString& name,
                const double* dispCoeff, int dispDispCoeffSize,
                const double* tempCoeff, int tempDispCoeffSize);

    enum{DispCoeffSize = 6};

    double get_refractive_index(double wavelen) const;

    int  setDispCoeff(const double* dispCoeff, int size);
    void setDispCoeff(double A, double B, double C,
                      double D, double E, double F);

    int getDispCoeff(double* dispCoeff, int size = DispCoeffSize) const;

private:
    double m_A;
    double m_B;
    double m_C;
    double m_D;
    double m_E;
    double m_F;
};

#endif // THERZBERGER_H
