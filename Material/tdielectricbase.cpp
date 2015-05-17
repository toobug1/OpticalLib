#include "tdielectricbase.h"

TDielectricBase::TDielectricBase() : m_formula(dfUnkonwn)
{

}

TDielectricBase::TDielectricBase(double temp) : TMaterialBase(temp), m_formula(dfUnkonwn)
{

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

double TDielectricBase::get_refractive_index(double wavelen) const
{
    return get_refractive_index(wavelen, m_temp, 1.0);
}



