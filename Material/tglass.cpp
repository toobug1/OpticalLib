#include "tglass.h"
#include "tschott.h"
#include "tsellmeier1.h"
#include "tsellmeier2.h"
#include "tsellmeier3.h"
#include "tsellmeier4.h"
#include "tsellmeier5.h"
#include "tconrady.h"
#include "textended.h"
#include "thandbook1.h"
#include "thandbook2.h"
#include "therzberger.h"


const TGlass none;

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
                                  dispCoeff, TSellmeier1::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Sellmeier2:
        m_glass = new TSellmeier2(mat.get_temperature(),
                                  mat.getName(),
                                  dispCoeff, TSellmeier2::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Sellmeier3:
        m_glass = new TSellmeier3(mat.get_temperature(),
                                  mat.getName(),
                                  dispCoeff, TSellmeier3::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Sellmeier4:
        m_glass = new TSellmeier4(mat.get_temperature(),
                                  mat.getName(),
                                  dispCoeff, TSellmeier4::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Sellmeier5:
        m_glass = new TSellmeier5(mat.get_temperature(),
                                  mat.getName(),
                                  dispCoeff, TSellmeier5::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Conrady:
        m_glass = new TConrady(mat.get_temperature(),
                               mat.getName(),
                               dispCoeff, TConrady::DispCoeffSize,
                               tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Extended:
        m_glass = new TExtended(mat.get_temperature(),
                                mat.getName(),
                                dispCoeff, TExtended::DispCoeffSize,
                                tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Handbook1:
        m_glass = new THandbook1(mat.get_temperature(),
                                 mat.getName(),
                                 dispCoeff, THandbook1::DispCoeffSize,
                                 tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Handbook2:
        m_glass = new THandbook2(mat.get_temperature(),
                                 mat.getName(),
                                 dispCoeff, THandbook2::DispCoeffSize,
                                 tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Herzberger:
        m_glass = new THerzberger(mat.get_temperature(),
                                  mat.getName(),
                                  dispCoeff, THerzberger::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Unkonwn:
        m_glass = 0;
        break;

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
                                  dispCoeff, TSellmeier1::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Sellmeier2:
        m_glass = new TSellmeier2(glass.m_glass->get_temperature(),
                                  glass.m_glass->getName(),
                                  dispCoeff, TSellmeier2::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Sellmeier3:
        m_glass = new TSellmeier3(glass.m_glass->get_temperature(),
                                  glass.m_glass->getName(),
                                  dispCoeff, TSellmeier3::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Sellmeier4:
        m_glass = new TSellmeier4(glass.m_glass->get_temperature(),
                                  glass.m_glass->getName(),
                                  dispCoeff, TSellmeier4::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Sellmeier5:
        m_glass = new TSellmeier5(glass.m_glass->get_temperature(),
                                  glass.m_glass->getName(),
                                  dispCoeff, TSellmeier5::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Conrady:
        m_glass = new TConrady(glass.m_glass->get_temperature(),
                               glass.m_glass->getName(),
                               dispCoeff, TConrady::DispCoeffSize,
                               tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Extended:
        m_glass = new TExtended(glass.m_glass->get_temperature(),
                                glass.m_glass->getName(),
                                dispCoeff, TExtended::DispCoeffSize,
                                tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Handbook1:
        m_glass = new THandbook1(glass.m_glass->get_temperature(),
                                 glass.m_glass->getName(),
                                 dispCoeff, THandbook1::DispCoeffSize,
                                 tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Handbook2:
        m_glass = new THandbook2(glass.m_glass->get_temperature(),
                                 glass.m_glass->getName(),
                                 dispCoeff, THandbook2::DispCoeffSize,
                                 tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Herzberger:
        m_glass = new THerzberger(glass.m_glass->get_temperature(),
                                  glass.m_glass->getName(),
                                  dispCoeff, THerzberger::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Unkonwn:
        m_glass = 0;
        break;

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

    case TDielectricBase::Sellmeier2:
        m_glass = new TSellmeier2(temp, name);
        break;

    case TDielectricBase::Sellmeier3:
        m_glass = new TSellmeier3(temp, name);
        break;

    case TDielectricBase::Sellmeier4:
        m_glass = new TSellmeier4(temp, name);
        break;

    case TDielectricBase::Sellmeier5:
        m_glass = new TSellmeier5(temp, name);
        break;

    case TDielectricBase::Conrady:
        m_glass = new TConrady(temp, name);
        break;

    case TDielectricBase::Extended:
        m_glass = new TExtended(temp, name);
        break;

    case TDielectricBase::Handbook1:
        m_glass = new THandbook1(temp, name);
        break;

    case TDielectricBase::Handbook2:
        m_glass = new THandbook2(temp, name);
        break;

    case TDielectricBase::Herzberger:
        m_glass = new THerzberger(temp, name);
        break;

    case TDielectricBase::Unkonwn:
        m_glass = 0;
        break;

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

    case TDielectricBase::Sellmeier2:
        m_glass = new TSellmeier2(temp, name, dispCoeff, dispDispCoeffSize,
                                  tempCoeff, tempDispCoeffSize);
        break;

    case TDielectricBase::Sellmeier3:
        m_glass = new TSellmeier3(temp, name, dispCoeff, dispDispCoeffSize,
                                  tempCoeff, tempDispCoeffSize);
        break;

    case TDielectricBase::Sellmeier4:
        m_glass = new TSellmeier4(temp, name, dispCoeff, dispDispCoeffSize,
                                  tempCoeff, tempDispCoeffSize);
        break;

    case TDielectricBase::Sellmeier5:
        m_glass = new TSellmeier5(temp, name, dispCoeff, dispDispCoeffSize,
                                  tempCoeff, tempDispCoeffSize);
        break;

    case TDielectricBase::Conrady:
        m_glass = new TConrady(temp, name, dispCoeff, dispDispCoeffSize,
                               tempCoeff, tempDispCoeffSize);
        break;

    case TDielectricBase::Extended:
        m_glass = new TExtended(temp, name, dispCoeff, dispDispCoeffSize,
                                tempCoeff, tempDispCoeffSize);
        break;

    case TDielectricBase::Handbook1:
        m_glass = new THandbook1(temp, name, dispCoeff, dispDispCoeffSize,
                                 tempCoeff, tempDispCoeffSize);
        break;

    case TDielectricBase::Handbook2:
        m_glass = new THandbook2(temp, name, dispCoeff, dispDispCoeffSize,
                                 tempCoeff, tempDispCoeffSize);
        break;

    case TDielectricBase::Herzberger:
        m_glass = new THerzberger(temp, name, dispCoeff, dispDispCoeffSize,
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
    if (!m_glass)
    {
        delete m_glass;
    }
    m_glass = 0;
}

TGlass& TGlass::operator=(const TGlass& glass)
{
    if (!m_glass)  // m_glass has been used
    {
        delete m_glass;
        m_glass = 0;
    }

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
                                  dispCoeff, TSellmeier1::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Sellmeier2:
        m_glass = new TSellmeier2(glass.m_glass->get_temperature(),
                                  glass.m_glass->getName(),
                                  dispCoeff, TSellmeier2::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Sellmeier3:
        m_glass = new TSellmeier3(glass.m_glass->get_temperature(),
                                  glass.m_glass->getName(),
                                  dispCoeff, TSellmeier3::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Sellmeier4:
        m_glass = new TSellmeier4(glass.m_glass->get_temperature(),
                                  glass.m_glass->getName(),
                                  dispCoeff, TSellmeier4::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Sellmeier5:
        m_glass = new TSellmeier5(glass.m_glass->get_temperature(),
                                  glass.m_glass->getName(),
                                  dispCoeff, TSellmeier5::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Conrady:
        m_glass = new TConrady(glass.m_glass->get_temperature(),
                               glass.m_glass->getName(),
                               dispCoeff, TConrady::DispCoeffSize,
                               tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Extended:
        m_glass = new TExtended(glass.m_glass->get_temperature(),
                                glass.m_glass->getName(),
                                dispCoeff, TExtended::DispCoeffSize,
                                tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Handbook1:
        m_glass = new THandbook1(glass.m_glass->get_temperature(),
                                 glass.m_glass->getName(),
                                 dispCoeff, THandbook1::DispCoeffSize,
                                 tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Handbook2:
        m_glass = new THandbook2(glass.m_glass->get_temperature(),
                                 glass.m_glass->getName(),
                                 dispCoeff, THandbook2::DispCoeffSize,
                                 tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Herzberger:
        m_glass = new THerzberger(glass.m_glass->get_temperature(),
                                  glass.m_glass->getName(),
                                  dispCoeff, THerzberger::DispCoeffSize,
                                  tempCoeff, TDielectricBase::TempCoeffSize);
        break;

    case TDielectricBase::Unkonwn:
        m_glass = 0;
        break;

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
    if (m_glass)
    {
        return m_glass->get_refractive_index(wave, temp, rlPressure);
    }
    else
    {
        return 1.0;  // for the empty material
    }
}

const TDielectricBase* TGlass::getGlassPtr() const
{
    return m_glass;
}

bool TGlass::isNull() const
{
    return !m_glass;
}

