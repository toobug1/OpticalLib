#ifndef TDIELECTRICBASE_H
#define TDIELECTRICBASE_H

#include "tmaterialbase.h"


class TDielectricBase : public TMaterialBase
{
    Q_OBJECT

    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(DispersionFormula formula READ formula WRITE setFormula)
    Q_PROPERTY(double tempD0 READ tempD0)
    Q_PROPERTY(double tempD1 READ tempD1)
    Q_PROPERTY(double tempD2 READ tempD2)
    Q_PROPERTY(double tempE0 READ tempE0)
    Q_PROPERTY(double tempE1 READ tempE1)
    Q_PROPERTY(double tempLtk READ tempLtk)
    Q_PROPERTY(double minWave READ minWave WRITE setMinWave)
    Q_PROPERTY(double maxWave READ maxWave WRITE setMaxWave)

public:
    enum DispersionFormula
    {
        Schott = 1, Sellmeier1, Herzberger, Sellmeier2,
        Conrady, Sellmeier3, Handbook1, Handbook2,
        Sellmeier4, Extended, Sellmeier5, Unkonwn = 0
    };

public:
    TDielectricBase();
    TDielectricBase(double temp, const QString& name);
    ~TDielectricBase();

    virtual bool is_opaque(double wavelen) const;
    virtual bool is_reflecting(double wavelen) const;

    virtual double get_refractive_index(double wavelen) const = 0;
    virtual double get_refractive_index(double wavelen, double temperature,
                                        double relativePressure = 1.0) const;

    QString getName() const {return m_name;}
    void setName(const QString& str) {m_name = str.toUpper();}
    DispersionFormula formula() const {return m_formula;}
    void setFormula(DispersionFormula formula) {m_formula = formula;}

    double tempD0() const {return m_D0;}
    double tempD1() const {return m_D1;}
    double tempD2() const {return m_D2;}
    double tempE0() const {return m_E0;}
    double tempE1() const {return m_E1;}
    double tempLtk() const {return m_Ltk;}

    enum{MinWave = 0, MaxWave = 10000};    // 0~10000nm

    double minWave() const {return m_MinWave;}
    void   setMinWave(double wave) {m_MinWave = wave;}
    double maxWave() const {return m_MaxWave;}
    void   setMaxWave(double wave) {m_MaxWave = wave;}
    bool   setWave(double minWave, double maxWave);

    enum {TempCoeffSize = 6};
    int  setTempCoeff(const double* tempCoeff, int size = TempCoeffSize);
    void setTempCoeff(double D0, double D1, double D2,
                      double E0, double E1, double Ltk);
    int  getTempCoeff(double* tempCoeff, int size = TempCoeffSize) const;

    virtual int setDispCoeff(const double* dispCoeff, int size);
    virtual int getDispCoeff(double* dispCoeff, int size) const;

protected:
    DispersionFormula  m_formula;

    double             m_MinWave;
    double             m_MaxWave;

    QString m_name;

private:
    void init(const QString& name = "");

private:
    double             m_D0;      // temperature Coeff.
    double             m_D1;
    double             m_D2;
    double             m_E0;
    double             m_E1;
    double             m_Ltk;
};

#endif // TDIELECTRICBASE_H
