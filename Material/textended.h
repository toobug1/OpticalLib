#ifndef TEXTENDED_H
#define TEXTENDED_H

#include "tdielectricbase.h"

class TExtended : public TDielectricBase
{
public:
    TExtended();
    virtual ~TExtended();

    TExtended(double temp, const QString& name);

    TExtended(double temp, const QString& name,
            const double* dispCoeff, int dispDispCoeffSize,
            const double* tempCoeff, int tempDispCoeffSize);

    enum{DispCoeffSize = 8};

    double get_refractive_index(double wavelen) const;

    int  setDispCoeff(const double* dispCoeff, int size);
    void setDispCoeff(double a0, double a1, double a2, double a3,
                      double a4, double a5, double a6, double a7);

    int getDispCoeff(double* dispCoeff, int size = DispCoeffSize) const;

private:
    double m_a0;
    double m_a1;
    double m_a2;
    double m_a3;
    double m_a4;
    double m_a5;
    double m_a6;
    double m_a7;
};

#endif // TEXTENDED_H
