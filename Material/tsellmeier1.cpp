#include "tsellmeier1.h"

#include "math.h"

TSellmeier1::TSellmeier1()
{
    m_formula = Sellmeier1;
}

TSellmeier1::TSellmeier1(double temp, const QString& name) : TDielectricBase(temp, name)
{
    m_formula = Sellmeier1;
}

TSellmeier1::TSellmeier1(double temp, const QString& name,
                         const double* dispCoeff, int dispDispCoeffSize,
                         const double* tempCoeff, int tempDispCoeffSize) :
    TDielectricBase(temp, name)
{
    m_formula = Sellmeier1;
    setDispCoeff(dispCoeff, dispDispCoeffSize);
    setTempCoeff(tempCoeff, tempDispCoeffSize);
}

TSellmeier1::~TSellmeier1()
{

}

double TSellmeier1::get_refractive_index(double wavelen) const
{
    if (wavelen < m_MinWave || wavelen > m_MaxWave)
    {
        return 0.0;
    }
    double Lamd = wavelen / 1000;
    double n;
    n = 	sqrt(m_K1*pow(Lamd, 2)/(pow(Lamd, 2) - m_L1) +
                 m_K2*pow(Lamd, 2)/(pow(Lamd, 2) - m_L2) +
                 m_K3*pow(Lamd, 2)/(pow(Lamd, 2) - m_L3) + 1);
    return n;
}

int  TSellmeier1::setDispCoeff(const double* dispCoeff, int size)
{
    if (size <= 0 || !dispCoeff)
    {
        return 0;
    }

    int count = size>DispCoeffSize? DispCoeffSize : size;

    switch(count)
    {
    case 6:
        m_K1 = dispCoeff[0];
        m_L1 = dispCoeff[1];
        m_K2 = dispCoeff[2];
        m_L2 = dispCoeff[3];
        m_K3 = dispCoeff[4];
        m_L3 = dispCoeff[5];
        break;

    case 5:
        m_K1 = dispCoeff[0];
        m_L1 = dispCoeff[1];
        m_K2 = dispCoeff[2];
        m_L2 = dispCoeff[3];
        m_K3 = dispCoeff[4];
        break;

    case 4:
        m_K1 = dispCoeff[0];
        m_L1 = dispCoeff[1];
        m_K2 = dispCoeff[2];
        m_L2 = dispCoeff[3];
        break;

    case 3:
        m_K1 = dispCoeff[0];
        m_L1 = dispCoeff[1];
        m_K2 = dispCoeff[2];
        break;

    case 2:
        m_K1 = dispCoeff[0];
        m_L1 = dispCoeff[1];
        break;

    case 1:
        m_K1 = dispCoeff[0];
        break;

    default:
        break;
    }

    return count;
}


void TSellmeier1::setDispCoeff(double K1, double L1, double K2,
                               double L2, double K3, double L3)
{
    double arr[DispCoeffSize] = {K1, L1, K2, L2, K3, L3};

    setDispCoeff(arr, DispCoeffSize);
}

int TSellmeier1::getDispCoeff(double* dispCoeff, int size) const
{
    if (size < DispCoeffSize)
    {
        return 0;
    }

    dispCoeff[0] = m_K1;
    dispCoeff[1] = m_L1;
    dispCoeff[2] = m_K2;
    dispCoeff[3] = m_L2;
    dispCoeff[4] = m_K3;
    dispCoeff[5] = m_L3;

    return DispCoeffSize;
}

