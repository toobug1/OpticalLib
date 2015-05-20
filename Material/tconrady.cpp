#include "tconrady.h"

#include "math.h"

TConrady::TConrady()
{
    m_formula = Conrady;
}

TConrady::TConrady(double temp, const QString& name) : TDielectricBase(temp, name)
{
    m_formula = Conrady;;
}

TConrady::TConrady(double temp, const QString& name,
                         const double* dispCoeff, int dispDispCoeffSize,
                         const double* tempCoeff, int tempDispCoeffSize) :
    TDielectricBase(temp, name)
{
    m_formula = Conrady;
    setDispCoeff(dispCoeff, dispDispCoeffSize);
    setTempCoeff(tempCoeff, tempDispCoeffSize);
}

TConrady::~TConrady()
{

}

double TConrady::get_refractive_index(double wavelen) const
{
    if (wavelen < m_MinWave || wavelen > m_MaxWave)
    {
        return 0.0;
    }
    double Lamd = wavelen / 1000;
    double n;
    n = m_n0 + m_A/Lamd + m_B/pow(Lamd, 3.5);
    return n;
}

int  TConrady::setDispCoeff(const double* dispCoeff, int size)
{
    if (size <= 0 || !dispCoeff)
    {
        return 0;
    }

    int count = size>DispCoeffSize? DispCoeffSize : size;

    switch(count)
    {
    case 3:
        m_n0 = dispCoeff[0];
        m_A = dispCoeff[1];
        m_B = dispCoeff[2];
        break;

    case 2:
        m_n0 = dispCoeff[0];
        m_A = dispCoeff[1];
        break;

    case 1:
        m_n0 = dispCoeff[0];
        break;

    default:
        break;
    }

    return count;
}

void TConrady::setDispCoeff(double n0, double A, double B)
{
    double arr[DispCoeffSize] = {n0, A, B};

    setDispCoeff(arr, DispCoeffSize);
}

int TConrady::getDispCoeff(double* dispCoeff, int size) const
{
    if (size < DispCoeffSize)
    {
        return 0;
    }

    dispCoeff[0] = m_n0;
    dispCoeff[1] = m_A;
    dispCoeff[2] = m_B;

    return DispCoeffSize;
}


