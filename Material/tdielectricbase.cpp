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

    m_D0 = 0.0;
    m_D1 = 0.0;
    m_D2 = 0.0;
    m_E0 = 0.0;
    m_E1 = 0.0;
    m_Ltk = 0.0;
    m_MinWave = MinWave;
    m_MaxWave = MaxWave;
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
        m_Ltk = 0.0;
        break;

    case 4:
        m_D0 = tempCoeff[0];
        m_D1 = tempCoeff[1];
        m_D2 = tempCoeff[2];
        m_E0 = tempCoeff[3];
        m_E1 = 0.0;
        m_Ltk = 0.0;
        break;

    case 3:
        m_D0 = tempCoeff[0];
        m_D1 = tempCoeff[1];
        m_D2 = tempCoeff[2];
        m_E0 = 0.0;
        m_E1 = 0.0;
        m_Ltk = 0.0;
        break;

    case 2:
        m_D0 = tempCoeff[0];
        m_D1 = tempCoeff[1];
        m_D2 = 0.0;
        m_E0 = 0.0;
        m_E1 = 0.0;
        m_Ltk = 0.0;
        break;

    case 1:
        m_D0 = tempCoeff[0];
        m_D1 = 0.0;
        m_D2 = 0.0;
        m_E0 = 0.0;
        m_E1 = 0.0;
        m_Ltk = 0.0;
        break;

    default:
        break;
    }

    return count;
}

void TDielectricBase::setTempCoeff(double D0, double D1, double D2,
                                   double E0, double E1, double Ltk)
{
    double arr[6] = {D0, D1, D2, E0, E1, Ltk};
    setTempCoeff(arr, 6);
}

int TDielectricBase::getTempCoeff(double* tempCoeff, int size) const
{
    if (size < TempCoeffSize)
    {
        return 0;
    }

    tempCoeff[0] = m_D0;
    tempCoeff[1] = m_D1;
    tempCoeff[2] = m_D2;
    tempCoeff[3] = m_E0;
    tempCoeff[4] = m_E1;
    tempCoeff[5] = m_Ltk;

    return TempCoeffSize;
}

bool TDielectricBase::setWave(double minWave, double maxWave)
{
    if (minWave > maxWave || minWave < MinWave || maxWave > MaxWave)
    {
        return false;
    }

    m_MinWave = minWave;
    m_MaxWave = maxWave;
    return true;
}


double TDielectricBase::get_refractive_index(double wavelen,
                                             double temperature,
                                             double relativePressure) const
{
    if (wavelen < m_MinWave || wavelen > m_MaxWave)
    {
        return 0.0;
    }

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

int TDielectricBase::setDispCoeff(const double* dispCoeff, int size)
{
    return 0;
}

int TDielectricBase::getDispCoeff(double* dispCoeff, int size) const
{
    return size;
}



