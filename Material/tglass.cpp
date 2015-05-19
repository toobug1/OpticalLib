#include "tglass.h"
#include "tschott.h"
#include "tsellmeier1.h"

TGlass::TGlass()
{
    m_glass = 0;
}

TGlass::TGlass(const TDielectricBase& mat)
{
    int size = MAXCoeffSize;
    double* dispCoeff = new double[size];
    mat.getDispCoeff(dispCoeff, size);
    double* tempCoeff = new double[size];
    mat.getTempCoeff(tempCoeff, size);

    switch (mat.formula())
    {
    case TDielectricBase::Schott:
        m_glass = new TSchott(mat.get_temperature(),
                              mat.getName(),
                              dispCoeff, TSchott::DispCoeffSize,
                              tempCoeff, TDielectricBase::TempCoeffSize);
        break;
    case TDielectricBase::Sellmeier1:
        m_glass = new TSellmeier1(mat.get_temperature(),
                              mat.getName(),
                              dispCoeff, TSchott::DispCoeffSize,
                              tempCoeff, TDielectricBase::TempCoeffSize);
        break;
    case TDielectricBase::Unkonwn:
        m_glass = 0;
        break;
        // TODO
    default:
        break;
    }
    if (!m_glass)
    {
        m_glass->setWave(mat.minWave(), mat.maxWave());
    }

    delete [] dispCoeff;
    delete [] tempCoeff;
}


TGlass::TGlass(const TGlass& glass)
{
    if (glass.isNull())
    {
        m_glass = 0;
        return;
    }

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

    case TDielectricBase::Sellmeier1:
        m_glass = new TSellmeier1(glass.m_glass->get_temperature(),
                              glass.m_glass->getName(),
                              dispCoeff, TSchott::DispCoeffSize,
                              tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Unkonwn:
        m_glass = 0;
        break;
        // TODO
    default:
        break;
    }
    if (!m_glass)
    {
        m_glass->setWave(glass.m_glass->minWave(), glass.m_glass->maxWave());
    }

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

    case TDielectricBase::Sellmeier1:
        m_glass = new TSellmeier1(temp, name);
        break;

    case TDielectricBase::Unkonwn:
        m_glass = 0;
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
        m_glass = new TSchott(temp, name, dispCoeff, dispDispCoeffSize,
                              tempCoeff, tempDispCoeffSize);
        break;

    case TDielectricBase::Sellmeier1:
        m_glass = new TSellmeier1(temp, name, dispCoeff, dispDispCoeffSize,
                              tempCoeff, tempDispCoeffSize);
        break;

    case TDielectricBase::Unkonwn:
        m_glass = 0;
        break;
    default:
        break;
    }
}

TGlass::~TGlass()
{
    delete m_glass;
}

TGlass& TGlass::operator=(const TGlass& glass)
{
    if (glass.isNull())
    {
        m_glass = 0;
        return *this;
    }

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

     case TDielectricBase::Sellmeier1:
        m_glass = new TSellmeier1(glass.m_glass->get_temperature(),
                              glass.m_glass->getName(),
                              dispCoeff, TSchott::DispCoeffSize,
                              tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Unkonwn:
        m_glass = 0;
        break;
        // TODO
    default:
        break;
    }
    if (!m_glass)
    {
        m_glass->setWave(glass.m_glass->minWave(), glass.m_glass->maxWave());
    }

    delete [] dispCoeff;
    delete [] tempCoeff;
    return *this;
}

double TGlass::getRefractiveIndex(double wave, double temp, double rlPressure)
{
    return m_glass->get_refractive_index(wave, temp, rlPressure);
}

const TDielectricBase* TGlass::getGlassPtr() const
{
    return m_glass;
}

bool TGlass::isNull() const
{
    return !getGlassPtr();
}

