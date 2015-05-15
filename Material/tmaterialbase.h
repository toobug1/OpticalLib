#ifndef TMATERIALBASE_H
#define TMATERIALBASE_H

#include <QMap>

class TMaterialBase
{
public:
    TMaterialBase();
    virtual ~TMaterialBase();

    virtual bool is_opaque(double wavelen) const = 0;
    virtual bool is_reflecting(double wavelen) const = 0;

    virtual double get_internal_transmittance(double wavelen, double thickness = 1.0) const;

    //absolute refractive index @wavelen in nm.
    virtual double get_refractive_index(double wavelen) const = 0;
    double get_refractive_index(double wavelen, const TMaterialBase &env) const;

    /* Get extinction coefficient. Subclasses _must_ provide this
        function or the get_internal_transmittance() function. */
    virtual double get_extinction_coef(double wavelen) const;

    virtual double get_normal_reflectance(const TMaterialBase *from, double wavelen) const;
    virtual double get_normal_transmittance(const TMaterialBase *from, double wavelen) const;

    void set_temperature(double temp);
    virtual double get_temperature() const;

    void insertAbsorbedCoeffi(double wavelen, double trans, double thichness);
    int removeAbsorbedCoeffi(double wavelen);

protected:
    double m_temp;        // celcius

    typedef double wavelength;
    typedef double absorptionCoefficient;
    QMap<wavelength, absorptionCoefficient> m_absorbMap;

protected:
    virtual double getInterpOfAbsorbCoeff(double wavelen) const;
    // wavelen shall be in [MinKey, MaxKey]
};

#endif // TMATERIALBASE_H