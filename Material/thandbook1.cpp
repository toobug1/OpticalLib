#include "thandbook1.h"

#include "math.h"

THandbook1::THandbook1()
{
    m_formula = Handbook1;
}

THandbook1::THandbook1(double temp, const QString& name) : TDielectricBase(temp, name)
{
    m_formula = Handbook1;
}

THandbook1::THandbook1(double temp, const QString& name,
                       const double* dispCoeff, int dispDispCoeffSize,
                       const double* tempCoeff, int tempDispCoeffSize) :
    TDielectricBase(temp, name)
{
    m_formula = Handbook1;
    setDispCoeff(dispCoeff, dispDispCoeffSize);
    setTempCoeff(tempCoeff, tempDispCoeffSize);
}

THandbook1::~THandbook1()
{

}

double THandbook1::get_refractive_index(double wavelen) const
{
    if (wavelen < m_MinWave || wavelen > m_MaxWave)
    {
        return 0.0;
    }
    double Lamd = wavelen / 1000;
    double n;
    n = sqrt(m_A + m_B / (pow(Lamd, 2) - m_C) - m_D * pow(Lamd, 2));
    return n;
}

int  THandbook1::setDispCoeff(const double* dispCoeff, int size)
{
    if (size <= 0 || !dispCoeff)
    {
        return 0;
    }

    int count = size>DispCoeffSize? DispCoeffSize : size;

    switch(count)
    {
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


void THandbook1::setDispCoeff(double A, double B, double C, double D)
{
    double arr[DispCoeffSize] = {A, B, C, D};

    setDispCoeff(arr, DispCoeffSize);
}

int THandbook1::getDispCoeff(double* dispCoeff, int size) const
{
    if (size < DispCoeffSize)
    {
        return 0;
    }

    dispCoeff[0] = m_A;
    dispCoeff[1] = m_B;
    dispCoeff[2] = m_C;
    dispCoeff[3] = m_D;

    return DispCoeffSize;
}

