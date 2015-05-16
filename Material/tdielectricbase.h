#ifndef TDIELECTRICBASE_H
#define TDIELECTRICVASE_H

#include "tmaterialbase.h"


class TDielectricBase : public TMaterialBase
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName)

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

private:
    QString m_name;

};

#endif // TDIELECTRICBASE_H
