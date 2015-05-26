#ifndef TRAY_H
#define TRAY_H

#include "Math/VectorPair"

using namespace _Goptical;
class TRay : public Math::VectorPair3
{
public:

    TRay();
    TRay(const Math::VectorPair3 &ray, double intensity = 1, double wavelen = 550);

    double get_intensity() const;
    void set_intensity(double intensity);

    double get_wavelen() const;
    void set_wavelen(double wavelen);

private:
    // nanometer
    double    _wavelen;
    // FIXME unit ?
    double    _intensity;
};

#endif // TRAY_H
