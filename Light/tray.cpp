#include "tray.h"

TRay::TRay()
{
}

TRay::TRay(const Math::VectorPair3 &ray,
           double intensity, double wavelen)
    : Math::VectorPair3(ray),
      _wavelen(wavelen),
      _intensity(intensity)
{
}

double TRay::get_intensity() const
{
    return _intensity;
}

void TRay::set_intensity(double intensity)
{
    _intensity = intensity;
}

double TRay::get_wavelen() const
{
    return _wavelen;
}

void TRay::set_wavelen(double wavelen)
{
    _wavelen = wavelen;
}


