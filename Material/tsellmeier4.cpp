#include "tsellmeier4.h"

#include "math.h"

TSellmeier4::TSellmeier4()
{
    m_formula = Sellmeier4;
}

TSellmeier4::TSellmeier4(double temp, const QString& name) : TDielectricBase(temp, name)
{
    m_formula = Sellmeier4;
}

TSellmeier4::TSellmeier4(double temp, const QString& name,
                       const double* dispCoeff, int dispDispCoeffSize,
                       const double* tempCoeff, int tempDispCoeffSize) :
    TDielectricBase(temp, name)
{
    m_formula = Sellmeier4;
    setDispCoeff(dispCoeff, dispDispCoeffSize);
    setTempCoeff(tempCoeff, tempDispCoeffSize);
}

TSellmeier4::~TSellmeier4()
{

}

double TSellmeier4::get_refractive_index(double wavelen) const
{
    if (wavelen < m_MinWave || wavelen > m_MaxWave)
    {
        return 0.0;
    }
    double Lamd = wavelen / 1000;
    double n;
    n = sqrt(m_A + m_B * pow(Lamd, 2)/(pow(Lamd, 2) - m_C) +
             m_D * pow(Lamd, 2)/(pow(Lamd, 2) - m_E));

    return n;
}

int  TSellmeier4::setDispCoeff(const double* dispCoeff, int size)
{
    if (size <= 0 || !dispCoeff)
    {
        return 0;
    }

    int count = size>DispCoeffSize? DispCoeffSize : size;

    switch(count)
    {
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


void TSellmeier4::setDispCoeff(double A, double B, double C, double D, double E)
{
    double arr[DispCoeffSize] = {A, B, C, D, E};

    setDispCoeff(arr, DispCoeffSize);
}

int TSellmeier4::getDispCoeff(double* dispCoeff, int size) const
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

    return DispCoeffSize;
}

