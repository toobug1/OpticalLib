#include "tair.h"
#include "math.h"

TAir::TAir()
{

}

TAir::TAir(double temp) : TMaterialBase(temp)
{

}

TAir::~TAir()
{

}

bool TAir::is_opaque(double wavelen) const
{
    return false;
}

bool TAir::is_reflecting(double wavelen) const
{
    return false;
}

double TAir::get_internal_transmittance(double wavelen, double thickness) const
{
    // there may be some formula
    // FIXME
    return 1.0;
}

double TAir::get_extinction_coef(double wavelen) const
{
    // there may be some formula
    // FIXME
    return 0.0;
}

double TAir::get_refractive_index(double wavelen) const
{
    return this->get_refractive_index(wavelen, m_temp, 1.0);
}

double TAir::get_refractive_index(double wavelen, double temp,
                                  double pressure) const
{

    double Nref, Nair;
    double powLamd = pow(wavelen/1000, 2.0);
    Nref = 1.0 + (6432.8 +
                 (2949810.0 * powLamd/ (146.0 * powLamd - 1.0)) +
                 (25540.0 * powLamd / (41.0 * powLamd - 1.0))) * 1e-8;

    Nair = 1.0 + (Nref - 1) * pressure/(1.0 + (temp - 15.0) * 3.4785 * 1e-3);

    return Nair;
}

QString TAir::getName() const
{
    return "Air";
}
void TAir::setName(const QString& name)
{
    return;
}

