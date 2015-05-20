#ifndef TCONRADY_H
#define TCONRADY_H

#include "tdielectricbase.h"

class TConrady : public TDielectricBase
{
public:
    TConrady();
    virtual ~TConrady();

    TConrady(double temp, const QString& name);

    TConrady(double temp, const QString& name,
                const double* dispCoeff, int dispDispCoeffSize,
                const double* tempCoeff, int tempDispCoeffSize);

    enum{DispCoeffSize = 3};

    double get_refractive_index(double wavelen) const;

    int  setDispCoeff(const double* dispCoeff, int size);
    void setDispCoeff(double n0, double A, double B);

    int getDispCoeff(double* dispCoeff, int size = DispCoeffSize) const;

private:
    double m_n0;
    double m_A;
    double m_B;
};

#endif // TCONRADY_H
