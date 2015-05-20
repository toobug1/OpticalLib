#include "tsellmeier5.h"

#include "math.h"

TSellmeier5::TSellmeier5()
{
    m_formula = Sellmeier5;
}

TSellmeier5::TSellmeier5(double temp, const QString& name) : TDielectricBase(temp, name)
{
    m_formula = Sellmeier5;
}

TSellmeier5::TSellmeier5(double temp, const QString& name,
                         const double* dispCoeff, int dispDispCoeffSize,
                         const double* tempCoeff, int tempDispCoeffSize) :
    TDielectricBase(temp, name)
{
    m_formula = Sellmeier5;
    setDispCoeff(dispCoeff, dispDispCoeffSize);
    setTempCoeff(tempCoeff, tempDispCoeffSize);
}

TSellmeier5::~TSellmeier5()
{

}

double TSellmeier5::get_refractive_index(double wavelen) const
{
    if (wavelen < m_MinWave || wavelen > m_MaxWave)
    {
        return 0.0;
    }
    double Lamd = wavelen / 1000;
    double n;
    n = sqrt(m_K1*pow(Lamd, 2)/(pow(Lamd, 2) - m_L1) +
             m_K2*pow(Lamd, 2)/(pow(Lamd, 2) - m_L2) +
             m_K3*pow(Lamd, 2)/(pow(Lamd, 2) - m_L3) +
             m_K4*pow(Lamd, 2)/(pow(Lamd, 2) - m_L4) +
             m_K5*pow(Lamd, 2)/(pow(Lamd, 2) - m_L5) + 1);
    return n;
}

int  TSellmeier5::setDispCoeff(const double* dispCoeff, int size)
{
    if (size <= 0 || !dispCoeff)
    {
        return 0;
    }

    int count = size>DispCoeffSize? DispCoeffSize : size;

    switch(count)
    {
    case 10:
        m_K1 = dispCoeff[0];
        m_L1 = dispCoeff[1];
        m_K2 = dispCoeff[2];
        m_L2 = dispCoeff[3];
        m_K3 = dispCoeff[4];
        m_L3 = dispCoeff[5];
        m_K4 = dispCoeff[6];
        m_L4 = dispCoeff[7];
        m_K5 = dispCoeff[8];
        m_L5 = dispCoeff[9];
        break;

    case 9:
        m_K1 = dispCoeff[0];
        m_L1 = dispCoeff[1];
        m_K2 = dispCoeff[2];
        m_L2 = dispCoeff[3];
        m_K3 = dispCoeff[4];
        m_L3 = dispCoeff[5];
        m_K4 = dispCoeff[6];
        m_L4 = dispCoeff[7];
        m_K5 = dispCoeff[8];
        break;

    case 8:
        m_K1 = dispCoeff[0];
        m_L1 = dispCoeff[1];
        m_K2 = dispCoeff[2];
        m_L2 = dispCoeff[3];
        m_K3 = dispCoeff[4];
        m_L3 = dispCoeff[5];
        m_K4 = dispCoeff[4];
        m_L4 = dispCoeff[5];
        break;

    case 7:
        m_K1 = dispCoeff[0];
        m_L1 = dispCoeff[1];
        m_K2 = dispCoeff[2];
        m_L2 = dispCoeff[3];
        m_K3 = dispCoeff[4];
        m_L3 = dispCoeff[5];
        m_K4 = dispCoeff[4];
        break;

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

void TSellmeier5::setDispCoeff(double K1, double L1, double K2, double L2,
                               double K3, double L3, double K4, double L4,
                               double K5, double L5)
{
    double arr[DispCoeffSize] = {K1, L1, K2, L2, K3, L3, K4, L4, K5, L5};

    setDispCoeff(arr, DispCoeffSize);
}

int TSellmeier5::getDispCoeff(double* dispCoeff, int size) const
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
    dispCoeff[6] = m_K4;
    dispCoeff[7] = m_L4;
    dispCoeff[8] = m_K5;
    dispCoeff[9] = m_L5;

    return DispCoeffSize;
}

