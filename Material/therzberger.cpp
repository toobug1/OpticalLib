#include "therzberger.h"

#include "math.h"

THerzberger::THerzberger()
{
    m_formula = Herzberger;
}

THerzberger::~THerzberger()
{

}


THerzberger::THerzberger(double temp, const QString& name) : TDielectricBase(temp, name)
{
    m_formula = Herzberger;
}

THerzberger::THerzberger(double temp, const QString& name,
                         const double* dispCoeff, int dispDispCoeffSize,
                         const double* tempCoeff, int tempDispCoeffSize) :
    TDielectricBase(temp, name)
{
    m_formula = Herzberger;
    setDispCoeff(dispCoeff, dispDispCoeffSize);
    setTempCoeff(tempCoeff, tempDispCoeffSize);
}

double THerzberger::get_refractive_index(double wavelen) const
{
    if (wavelen < m_MinWave || wavelen > m_MaxWave)
    {
        return 0.0;
    }
    double Lamd = wavelen / 1000;
    double L = 1/(pow(Lamd, 2) - 0.028);
    double n;
    n = m_A + m_B * L + m_C * pow(L, 2) + m_D * pow(Lamd, 2) +
        m_E * pow(Lamd, 4) + m_F * pow(Lamd, 6);

    return n;
}

int  THerzberger::setDispCoeff(const double* dispCoeff, int size)
{
    if (size <= 0 || !dispCoeff)
    {
        return 0;
    }

    int count = size>DispCoeffSize? DispCoeffSize : size;

    switch(count)
    {
    case 6:
        m_A = dispCoeff[0];
        m_B = dispCoeff[1];
        m_C = dispCoeff[2];
        m_D = dispCoeff[3];
        m_E = dispCoeff[4];
        m_F = dispCoeff[5];
        break;

    case 5:
        m_A = dispCoeff[0];
        m_B = dispCoeff[1];
        m_C = dispCoeff[2];
        m_D = dispCoeff[3];
        m_E = dispCoeff[4];
        break;

    case 4:
        m_A = dispCoeff[0];
        m_B = dispCoeff[1];
        m_C = dispCoeff[2];
        m_D = dispCoeff[3];
        break;

    case 3:
        m_A = dispCoeff[0];
        m_B = dispCoeff[1];
        m_C = dispCoeff[2];
        break;

    case 2:
        m_A = dispCoeff[0];
        m_B = dispCoeff[1];
        break;

    case 1:
        m_A = dispCoeff[0];
        break;

    default:
        break;
    }

    return count;
}


void THerzberger::setDispCoeff(double A, double B, double C,
                               double D, double E, double F)
{
    double arr[DispCoeffSize] = {A, B, C, D, E, F};

    setDispCoeff(arr, DispCoeffSize);
}

int THerzberger::getDispCoeff(double* dispCoeff, int size) const
{
    if (size < DispCoeffSize)
    {
        return 0;
    }

    dispCoeff[0] = m_A;
    dispCoeff[1] = m_B;
    dispCoeff[2] = m_C;
    dispCoeff[3] = m_D;
    dispCoeff[4] = m_E;
    dispCoeff[5] = m_F;

    return DispCoeffSize;
}

