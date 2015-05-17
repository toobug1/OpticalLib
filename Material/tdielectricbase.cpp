#include "tdielectricbase.h"

TDielectricBase::TDielectricBase()
{

}

TDielectricBase::TDielectricBase(double temp) : TMaterialBase(temp)
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



