#ifndef TMATERIALBASE_H
#define TMATERIALBASE_H

#include <QMap>
#include <QObject>

class TMaterialBase : public QObject
{
    Q_OBJECT

public:
    TMaterialBase();
    TMaterialBase(double temp);
    virtual ~TMaterialBase();

    virtual bool is_opaque(double wavelen) const = 0;
    virtual bool is_reflecting(double wavelen) const = 0;

    virtual double get_internal_transmittance(double wavelen, double thickness = 1.0) const;

    //absolute refractive index @wavelen in nm.
    virtual double get_refractive_index(double wavelen) const = 0;
    virtual double get_refractive_index(double wavelen, double temperature,
                                double relativePressure) const = 0;

    /* Get extinction coefficient. Subclasses _must_ provide this
        function or the get_internal_transmittance() function. */
    virtual double get_extinction_coef(double wavelen) const;

    virtual double get_normal_reflectance(const TMaterialBase *from, double wavelen) const;
    virtual double get_normal_transmittance(const TMaterialBase *from, double wavelen) const;

    void set_temperature(double temp);
    double get_temperature() const;
    virtual QString getName() const = 0;
    virtual void setName(const QString& name) = 0;

    void insertAbsorbedCoeffi(double wavelen, double trans, double thichness);
    int removeAbsorbedCoeffi(double wavelen);
    void clearAbsorbedCoeffi();

    typedef double wavelength;
    typedef double absorptionCoefficient;

    QMap<double, double> get_trans_table() const;


protected:
    double m_temp;        // celcius
    QMap<wavelength, absorptionCoefficient> m_absorbMap;

protected:
    double getInterpOfAbsorbCoeff(double wavelen) const;
    // wavelen shall be among [MinKey, MaxKey]
};

#endif // TMATERIALBASE_H
