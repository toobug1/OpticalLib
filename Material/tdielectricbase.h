#ifndef TDIELECTRICBASE_H
#define TDIELECTRICVASE_H

#include "tmaterialbase.h"


class TDielectricBase : public TMaterialBase
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(DispersionFormula formula READ formula)
    Q_PROPERTY(double tempD0 READ tempD0)
    Q_PROPERTY(double tempD1 READ tempD1)
    Q_PROPERTY(double tempD2 READ tempD2)
    Q_PROPERTY(double tempE0 READ tempE0)
    Q_PROPERTY(double tempE1 READ tempE1)
    Q_PROPERTY(double tempLtk READ tempLtk)
    Q_PROPERTY(double minWave READ minWave)
    Q_PROPERTY(double maxWave READ maxWave)

public:
    enum DispersionFormula
    {
        dfSchott = 1,
        dfSellmeier1,
        dfHerzberger,
        dfSellmeier2,
        dfConrady,
        dfSellmeier3,
        dfHandbook1,
        dfHandbook2,
        dfSellmeier4,
        dfExtended,
        dfSellmeier5,
        dfUnkonwn = 0
    };

public:
    TDielectricBase();
    TDielectricBase(double temp);
    ~TDielectricBase();

    virtual bool is_opaque(double wavelen) const;
    virtual bool is_reflecting(double wavelen) const;

    virtual double get_refractive_index(double wavelen) const;
    virtual double get_refractive_index(double wavelen, double temperature,
                                        double relativePressure) const = 0;
    // derived class for this needs to impliment.
    // and need a class which includes this base class pointer.
    // it's constructor function with a specialized tag parameter
    // that indicates which is the material type.

    QString name() const {return m_name;}
    void setName(const QString& str) {m_name = str.toUpper();}
    DispersionFormula formula() const {return m_formula;}
    double tempD0() const {return m_D0;}
    double tempD1() const {return m_D1;}
    double tempD2() const {return m_D2;}
    double tempE0() const {return m_E0;}
    double tempE1() const {return m_E1;}
    double tempLtk() const {return m_Ltk;}
    double minWave() const {return m_MinWave;}
    double maxWave() const {return m_MaxWave;}

protected:
    DispersionFormula m_formula;
    double             m_D0;      // temperature Coeff.
    double             m_D1;
    double             m_D2;
    double             m_E0;
    double             m_E1;
    double             m_Ltk;
    double             m_MinWave;
    double             m_MaxWave;

    QString m_name;
};

#endif // TDIELECTRICBASE_H
