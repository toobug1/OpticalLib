#ifndef TGLASS_H
#define TGLASS_H

#include "tdielectricbase.h"

// This class is similar a factory class which can creat a sort of material


class TGlass
{
public:
    TGlass();

    TGlass(const TGlass& glass);

    TGlass(TDielectricBase::DispersionFormula df,
           double temp, const QString& name);

    TGlass(TDielectricBase::DispersionFormula df,
           double temp, const QString& name,
           const double* dispCoeff, int dispDispCoeffSize,
           const double* tempCoeff, int tempDispCoeffSize);

    TGlass(const TDielectricBase& mat);

    ~TGlass();

    TGlass& operator=(const TGlass& glass);

    double getRefractiveIndex(double wave, double temp, double rlPressure);

    const TDielectricBase* getGlassPtr() const;

    bool isNull() const {return getGlassPtr();}

private:
    TDielectricBase* m_glass;
};

#endif // TGLASS_H
