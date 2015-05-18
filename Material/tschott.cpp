#include "tschott.h"

#include "math.h"

TSchott::TSchott()
{
    m_formula = dfSchott;
}

TSchott::TSchott(double temp, const QString name) : TDielectricBase(temp, name)
{
    m_formula = dfSchott;
}

TSchott::~TSchott()
{

}

double TSchott::get_refractive_index(double wavelen) const
{
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


