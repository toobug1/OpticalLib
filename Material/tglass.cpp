#include "tglass.h"
#include "tschott.h"

TGlass::TGlass()
{
    m_glass = 0;
}

TGlass::TGlass(const TGlass& glass)
{
    if (glass.isNull())
    {
        m_glass = 0;
        return;
    }

    // get

    int size = 10;
    double* dispCoeff = new double[size];
    glass.m_glass->getDispCoeff(dispCoeff, size);
    double* tempCoeff = new double[size];
    glass.m_glass->getTempCoeff(tempCoeff, size);

    switch (glass.m_glass->formula())
    {
    case TDielectricBase::Schott:
        m_glass = new TSchott(glass.m_glass->get_temperature(),
                              glass.m_glass->getName(),
                              dispCoeff, TSchott::DispCoeffSize,
                              tempCoeff, TDielectricBase::TempCoeffSize);
        break;
        // TODO
    default:
        break;
    }

    m_glass->setWave(glass.m_glass->minWave(), glass.m_glass->maxWave());

    delete [] dispCoeff;
    delete [] tempCoeff;
}

TGlass::TGlass(TDielectricBase::DispersionFormula df,
               double temp, const QString& name)
{
    switch (df)
    {
    case TDielectricBase::Schott:
        m_glass = new TSchott(temp, name);
        break;
        // TODO
    default:
        break;
    }
}

TGlass::TGlass(TDielectricBase::DispersionFormula df,
               double temp, const QString& name,
               const double* dispCoeff, int dispDispCoeffSize,
               const double* tempCoeff, int tempDispCoeffSize)
{
    switch (df)
    {
    case TDielectricBase::Schott:
        m_glass = new TSchott(temp, name,
                              dispCoeff, dispDispCoeffSize,
                              tempCoeff, tempDispCoeffSize);
        break;
    default:
        break;
    }
}

TGlass::~TGlass()
{
    delete m_glass;
}


const TDielectricBase* TGlass::getGlassPtr() const
{
    return m_glass;
}

