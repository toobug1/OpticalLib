#ifndef TSPECTRALLINE_H
#define TSPECTRALLINE_H

#include "Io/trgb.h"

class TSpectralLine
{
public:
    /** Create a spectral line with specified wavelen and
      intensity. */
    TSpectralLine(double wavelen, double intensity = 1.0);

    /** get spectral line wavelen */
    double get_wavelen() const;
    /** get spectral line intensity */
    double get_intensity() const;
    /** get rgb color associated spectral line, ignore intensity */
    TRgb get_color() const;
    /** get rgb color associated with wavelen */
    static TRgb get_wavelen_color(double wl);

    /** red colored line at 645nm */
    static const double red;
    /** green colored line at 510nm */
    static const double green;
    /** blue colored line at 440nm */
    static const double blue;

    /** infrared mercury line at 1013.98nm */
    static const double t;
    /** infrared cesium line at 852.11nm */
    static const double s;
    /** red helium line at 706.5188nm */
    static const double r;
    /** red hydrogen line at 656.2725nm */
    static const double C;
    /** red cadmium line at 643.8469nm */
    static const double C_; // C'
    /** yellow sodium line at 589.2938nm */
    static const double D;
    /** yellow helium line at 587.5618nm */
    static const double d;
    /** green mercury line at 546.074nm */
    static const double e;
    /** blue hydrogen line at 486.1327nm */
    static const double F;
    /** blue cadmium line at 479.9914nm */
    static const double F_; // F'
    /** blue mercury line at 435.8343nm */
    static const double g;
    /** violet mercury line at 404.6561nm */
    static const double h;
    /** ultraviolet mercury line at 365.0146nm */
    static const double i;

private:
    double    _wavelen;
    double    _intensity;
};

#endif // TSPECTRALLINE_H
