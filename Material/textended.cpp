#include "textended.h"

#include "math.h"

TExtended::TExtended()
{
    m_formula = Extended;
}

TExtended::TExtended(double temp, const QString& name) : TDielectricBase(temp, name)
{
    m_formula = Extended;
}

TExtended::TExtended(double temp, const QString& name,
                     const double* dispCoeff, int dispDispCoeffSize,
                     const double* tempCoeff, int tempDispCoeffSize) :
    TDielectricBase(temp, name)
{
    m_formula = Extended;
    setDispCoeff(dispCoeff, dispDispCoeffSize);
    setTempCoeff(tempCoeff, tempDispCoeffSize);
}

TExtended::~TExtended()
{

}

double TExtended::get_refractive_index(double wavelen) const
{
    if (wavelen < m_MinWave || wavelen > m_MaxWave)
    {
        return 0.0;
    }
    double Lamd = wavelen / 1000;
    double n;
    n = sqrt(m_a0 +
             m_a1 * pow(Lamd,2) +
             m_a2 * pow(Lamd, -2) +
             m_a3 * pow(Lamd, -4) +
             m_a4 * pow(Lamd, -6) +
             m_a5 * pow(Lamd, -8) +
             m_a6 * pow(Lamd, -10) +
             m_a7 * pow(Lamd, -12));
    return n;
}

int  TExtended::setDispCoeff(const double* dispCoeff, int size)
{
    if (size <= 0 || !dispCoeff)
    {
        return 0;
    }

    int count = size>DispCoeffSize? DispCoeffSize : size;

    switch(count)
    {
    case 8:
        m_a0 = dispCoeff[0];
        m_a1 = dispCoeff[1];
        m_a2 = dispCoeff[2];
        m_a3 = dispCoeff[3];
        m_a4 = dispCoeff[4];
        m_a5 = dispCoeff[5];
        m_a6 = dispCoeff[4];
        m_a7 = dispCoeff[5];
        break;

    case 7:
        m_a0 = dispCoeff[0];
        m_a1 = dispCoeff[1];
        m_a2 = dispCoeff[2];
        m_a3 = dispCoeff[3];
        m_a4 = dispCoeff[4];
        m_a5 = dispCoeff[5];
        m_a6 = dispCoeff[4];
        break;

    case 6:
        m_a0 = dispCoeff[0];
        m_a1 = dispCoeff[1];
        m_a2 = dispCoeff[2];
        m_a3 = dispCoeff[3];
        m_a4 = dispCoeff[4];
        m_a5 = dispCoeff[5];
        break;

    case 5:
        m_a0 = dispCoeff[0];
        m_a1 = dispCoeff[1];
        m_a2 = dispCoeff[2];
        m_a3 = dispCoeff[3];
        m_a4 = dispCoeff[4];
        break;

    case 4:
        m_a0 = dispCoeff[0];
        m_a1 = dispCoeff[1];
        m_a2 = dispCoeff[2];
        m_a3 = dispCoeff[3];
        break;

    case 3:
        m_a0 = dispCoeff[0];
        m_a1 = dispCoeff[1];
        m_a2 = dispCoeff[2];
        break;

    case 2:
        m_a0 = dispCoeff[0];
        m_a1 = dispCoeff[1];
        break;

    case 1:
        m_a0 = dispCoeff[0];
        break;

    default:
        break;
    }

    return count;
}

void TExtended::setDispCoeff(double a0, double a1, double a2, double a3,
                             double a4, double a5, double a6, double a7)
{
    double arr[DispCoeffSize] = {a0, a1, a2, a3, a4, a5, a6, a7};

    setDispCoeff(arr, DispCoeffSize);
}

int TExtended::getDispCoeff(double* dispCoeff, int size) const
{
    if (size < DispCoeffSize)
    {
        return 0;
    }

    dispCoeff[0] = m_a0;
    dispCoeff[1] = m_a1;
    dispCoeff[2] = m_a2;
    dispCoeff[3] = m_a3;
    dispCoeff[4] = m_a4;
    dispCoeff[5] = m_a5;
    dispCoeff[6] = m_a6;
    dispCoeff[7] = m_a7;

    return DispCoeffSize;
}

