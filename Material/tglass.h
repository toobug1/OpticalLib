#ifndef TGLASS_H
#define TGLASS_H

#include "tdielectricbase.h"

// This class is similar a factory class which can creat a sort of material


class TGlass
{
public:
    TGlass();
    TGlass(const TGlass& glass);
    TGlass(const TDielectricBase& mat);

    TGlass(TDielectricBase::DispersionFormula df,
           double temp, const QString& name);

    TGlass(TDielectricBase::DispersionFormula df,
           double temp, const QString& name,
           const double* dispCoeff, int dispDispCoeffSize,
           const double* tempCoeff, int tempDispCoeffSize);

    virtual ~TGlass();
    TGlass& operator=(const TGlass& glass);

    double getRefractiveIndex(double wave, double temp, double rlPressure = 1.0);
    const TDielectricBase* getGlassPtr() const;
    bool isNull() const;

    enum {MAXCoeffSize = 10};

private:
    TDielectricBase* m_glass;
};

extern const TGlass none;

#endif // TGLASS_H
