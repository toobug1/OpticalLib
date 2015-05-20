#include "tsellmeier2.h"

#include "math.h"

TSellmeier2::TSellmeier2()
{
    m_formula = Sellmeier2;
}

TSellmeier2::TSellmeier2(double temp, const QString& name) : TDielectricBase(temp, name)
{
    m_formula = Sellmeier2;
}

TSellmeier2::TSellmeier2(double temp, const QString& name,
                         const double* dispCoeff, int dispDispCoeffSize,
                         const double* tempCoeff, int tempDispCoeffSize) :
    TDielectricBase(temp, name)
{
    m_formula = Sellmeier2;
    setDispCoeff(dispCoeff, dispDispCoeffSize);
    setTempCoeff(tempCoeff, tempDispCoeffSize);
}

TSellmeier2::~TSellmeier2()
{

}

double TSellmeier2::get_refractive_index(double wavelen) const
{
    if (wavelen < m_MinWave || wavelen > m_MaxWave)
    {
        return 0.0;
    }
    double Lamd = wavelen / 1000;
    double n;
    n = sqrt(m_A + m_B1 * pow(Lamd, 2)/ (pow(Lamd, 2) - pow(m_Lamd1, 2)) +
             m_B2 * pow(Lamd, 2) / (pow(Lamd, 2) - pow(m_Lamd2, 2)) + 1);
    return n;
}

int  TSellmeier2::setDispCoeff(const double* dispCoeff, int size)
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
        m_B1 = dispCoeff[1];
        m_B2 = dispCoeff[2];
        m_Lamd1 = dispCoeff[3];
        m_Lamd2 = dispCoeff[4];
        break;

    case 4:
        m_A = dispCoeff[0];
        m_B1 = dispCoeff[1];
        m_B2 = dispCoeff[2];
        m_Lamd1 = dispCoeff[3];
        break;

    case 3:
        m_A = dispCoeff[0];
        m_B1 = dispCoeff[1];
        m_B2 = dispCoeff[2];
        break;

    case 2:
        m_A = dispCoeff[0];
        m_B1 = dispCoeff[1];
        break;

    case 1:
        m_A = dispCoeff[0];
        break;

    default:
        break;
    }

    return count;
}

void TSellmeier2::setDispCoeff(double A, double B1, double B2,
                               double Lamd1, double Lamd2)
{
    double arr[DispCoeffSize] = {A, B1, B2, Lamd1, Lamd2};

    setDispCoeff(arr, DispCoeffSize);
}

int TSellmeier2::getDispCoeff(double* dispCoeff, int size) const
{
    if (size < DispCoeffSize)
    {
        return 0;
    }

    dispCoeff[0] = m_A;
    dispCoeff[1] = m_B1;
    dispCoeff[2] = m_B2;
    dispCoeff[3] = m_Lamd1;
    dispCoeff[4] = m_Lamd2;

    return DispCoeffSize;
}
