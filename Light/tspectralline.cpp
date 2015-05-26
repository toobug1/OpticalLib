#include "tspectralline.h"

double TSpectralLine::get_wavelen() const
{
  return _wavelen;
}

double TSpectralLine::get_intensity() const
{
  return _intensity;
}

TRgb TSpectralLine::get_color() const
{
  return get_wavelen_color(_wavelen);
}

TSpectralLine::TSpectralLine(double wavelen, double intensity)
  : _wavelen(wavelen), _intensity(intensity)
{
}

TRgb TSpectralLine::get_wavelen_color(double wl)
{
  // based on algorithm from Dan Bruton (www.physics.sfasu.edu/astro/color.html)

  if (wl < 380.0 || wl > 780.0)
    return rgb_black;

  float s = 1.0;

  if (wl < 420.0)
    s = 0.3 + 0.7 * (wl - 380.0) / 40.0;
  else if (wl > 700.0)
    s = 0.3 + 0.7 * (780.0 - wl) / 80.0;

  if (wl < 510.0)
    {
      if (wl < 490.0)
        {
          if (wl < 440.0)
            // 380 to 440
            return TRgb(s * -(wl - 440.0) / 60.0, 0.0, s);
          else
            // 440 to 490
            return TRgb(0.0, s * (wl - 440.0) / 50.0, s);
        }
      else
        // 490 to 510
        return TRgb(0.0, s, s * -(wl - 510.0) / 20.0);
    }
  else
    {
      if (wl < 645.0)
        {
          if (wl < 580.0)
            // 510 to 580
            return TRgb(s * (wl - 510.0) / 70.0, s, 0.0);
          else
            // 580 to 645
            return TRgb(s, s * -(wl - 645.0) / 65.0, 0.0);
        }
      else
        {
          // 645 to 780
          return TRgb(s, 0.0, 0.0);
        }
    }
}

const double TSpectralLine::red = 645.0;
const double TSpectralLine::green = 510.0;
const double TSpectralLine::blue = 440.0;
const double TSpectralLine::t = 1013.98;
const double TSpectralLine::s = 852.11;
const double TSpectralLine::r = 706.5188;
const double TSpectralLine::C = 656.2725;
const double TSpectralLine::C_ = 643.8469; // C'
const double TSpectralLine::D = 589.2938;
const double TSpectralLine::d = 587.5618;
const double TSpectralLine::e = 546.074;
const double TSpectralLine::F = 486.1327;
const double TSpectralLine::F_ = 479.9914; // F'
const double TSpectralLine::g = 435.8343;
const double TSpectralLine::h = 404.6561;
const double TSpectralLine::i = 365.0146;

