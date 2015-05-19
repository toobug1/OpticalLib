#include "tschott.h"

#include "math.h"

TSchott::TSchott()
{
    m_formula = Schott;
}

TSchott::TSchott(double temp, const QString& name) : TDielectricBase(temp, name)
{
    m_formula = Schott;
}

TSchott::TSchott(double temp, const QString& name,
        const double* dispCoeff, int dispDispCoeffSize,
        const double* tempCoeff, int tempDispCoeffSize) :
    TDielectricBase(temp, name)
{
    m_formula = Schott;
    setDispCoeff(dispCoeff, dispDispCoeffSize);
    setTempCoeff(tempCoeff, tempDispCoeffSize);
}

//TSchott::TSchott(double temp, const QString& name,
//        double A0, double A1, double A2,
//        double A3, double A4, double A5,
//        double D0, double D1, double D2,
//        double E0, double E1, double Ltk) :
//    TDielectricBase(temp, name)
//{
//    m_formula = Schott;
//    double dispCoeff[6] = {A0, A1, A2, A3, A4, A5};
//    double tempCoeff[6] = {D0, D1, D2, E0, E1, Ltk};

//    setDispCoeff(dispCoeff, 6);
//    setTempCoeff(tempCoeff, 6);
//}

TSchott::~TSchott()
{
}

double TSchott::get_refractive_index(double wavelen) const
{
    if (wavelen < m_MinWave || wavelen > m_MaxWave)
    {
        return 0.0;
    }

    double n;
    double Lamd = wavelen / 1000.0;
    n = sqrt(m_A0 +
             m_A1*pow(Lamd,2) +
             m_A2*pow(Lamd, -2) +
             m_A3*pow(Lamd, -4) +
             m_A4*pow(Lamd, -6) +
             m_A5*pow(Lamd, -8));
    return n;
}

int TSchott::setDispCoeff(const double* dispCoeff, int size)
{
    if (size <= 0 || !dispCoeff)
    {
        return 0;
    }

    int count = size>DispCoeffSize? DispCoeffSize : size;

    switch(count)
    {
    case 6:
        m_A0 = dispCoeff[0];
        m_A1 = dispCoeff[1];
        m_A2 = dispCoeff[2];
        m_A3 = dispCoeff[3];
        m_A4 = dispCoeff[4];
        m_A5 = dispCoeff[5];
        break;

    case 5:
        m_A0 = dispCoeff[0];
        m_A1 = dispCoeff[1];
        m_A2 = dispCoeff[2];
        m_A3 = dispCoeff[3];
        m_A4 = dispCoeff[4];
        break;

    case 4:
        m_A0 = dispCoeff[0];
        m_A1 = dispCoeff[1];
        m_A2 = dispCoeff[2];
        m_A3 = dispCoeff[3];
        break;

    case 3:
        m_A0 = dispCoeff[0];
        m_A1 = dispCoeff[1];
        m_A2 = dispCoeff[2];
        break;

    case 2:
        m_A0 = dispCoeff[0];
        m_A1 = dispCoeff[1];
        break;

    case 1:
        m_A0 = dispCoeff[0];
        break;

    default:
        break;
    }

    return count;
}

void TSchott::setDispCoeff(double A0, double A1, double A2,
                 double A3, double A4, double A5)
{
    double arr[DispCoeffSize] = {A0, A1, A2, A3, A4, A5};

    setDispCoeff(arr, DispCoeffSize);
}

int TSchott::getDispCoeff(double* dispCoeff, int size) const
{
    if (size < DispCoeffSize)
    {
        return 0;
    }

    dispCoeff[0] = m_A0;
    dispCoeff[1] = m_A1;
    dispCoeff[2] = m_A2;
    dispCoeff[3] = m_A3;
    dispCoeff[4] = m_A4;
    dispCoeff[5] = m_A5;

    return DispCoeffSize;
}


