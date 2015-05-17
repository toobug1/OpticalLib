#include "tschott.h"

#include "math.h"

TSchott::TSchott()
{
    m_formula = dfSchott;
}

TSchott::TSchott(double temp) : TDielectricBase(temp)
{
    m_formula = dfSchott;
}

TSchott::~TSchott()
{

}

double TSchott::get_refractive_index(double wavelen, double temperature,
                                     double relativePressure) const
{
    double n_Tref;
    double Lamd = wavelen / 1000.0;

    n_Tref =sqrt(A0 +
                 A1*pow(Lamd,2) +
                 A2*pow(Lamd, -2) +
                 A3*pow(Lamd, -4) +
                 A4*pow(Lamd, -6) +
                 A5*pow(Lamd, -8));

    // TODO
}
