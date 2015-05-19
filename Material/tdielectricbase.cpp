#include "tdielectricbase.h"
#include "tair.h"

#include "math.h"

TDielectricBase::TDielectricBase()
{

    init();
}

TDielectricBase::TDielectricBase(double temp, const QString& name) :
    TMaterialBase(temp)
{
    init(name);
}

TDielectricBase::~TDielectricBase()
{

}

bool TDielectricBase::is_opaque(double wavelen) const
{
    return false;
}
bool TDielectricBase::is_reflecting(double wavelen) const
{
    return false;
}



void TDielectricBase::init(const QString& name)
{
    m_name = name.toUpper();
    //    m_formula = dfUnkonwn;
    m_D0 = 0.0;
    m_D1 = 0.0;
    m_D2 = 0.0;
    m_E0 = 0.0;
    m_E1 = 0.0;
    m_Ltk = 0.0;
    m_MinWave = 0.0;
    m_MaxWave = 0.0;
}


int TDielectricBase::setTempCoeff(const double* tempCoeff, int size)
{
    if (size <= 0 || !tempCoeff)
    {
        return 0;
    }

    int count = size>6? 6 : size;

    switch(count)
    {
    case 6:
        m_D0 = tempCoeff[0];
        m_D1 = tempCoeff[1];
        m_D2 = tempCoeff[2];
        m_E0 = tempCoeff[3];
        m_E1 = tempCoeff[4];
        m_Ltk = tempCoeff[5];
        break;

    case 5:
        m_D0 = tempCoeff[0];
        m_D1 = tempCoeff[1];
        m_D2 = tempCoeff[2];
        m_E0 = tempCoeff[3];
        m_E1 = tempCoeff[4];
        break;

    case 4:
        m_D0 = tempCoeff[0];
        m_D1 = tempCoeff[1];
        m_D2 = tempCoeff[2];
        m_E0 = tempCoeff[3];
        break;

    case 3:
        m_D0 = tempCoeff[0];
        m_D1 = tempCoeff[1];
        m_D2 = tempCoeff[2];
        break;

    case 2:
        m_D0 = tempCoeff[0];
        m_D1 = tempCoeff[1];
        break;

    case 1:
        m_D0 = tempCoeff[0];
        break;

    default:
        break;
    }

    return count;
}


double TDielectricBase::get_refractive_index(double wavelen, double temperature,
                                    double relativePressure) const
{
    double n_Tref;
    double Lamd = wavelen / 1000.0;

    // get refrative index at std. temp and std pressure
    n_Tref = get_refractive_index(wavelen);

    TAir air(m_temp);
    double n_air_std = air.get_refractive_index(wavelen);

    // glass refractive index vs vacuum at std temp.
    double n_abs_ref = n_Tref * n_air_std;

    double dltT = temperature - m_temp;

    double dltNabs = (pow(n_Tref, 2) - 1)/(2 * n_Tref)*
                     (m_D0*dltT + m_D1*pow(dltT,2) + m_D2*pow(dltT,3) +
                     (m_E0*dltT + m_E1*pow(dltT,2))/(pow(Lamd,2)-pow(m_Ltk,2)));

    // refractive at vacuum and temp.
    double n_abs_T = n_abs_ref + dltNabs;

    double n_air_T = air.get_refractive_index(wavelen, temperature,
                                              relativePressure);

    return  n_abs_T / n_air_T;

}


