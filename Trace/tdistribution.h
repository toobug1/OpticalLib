#ifndef TDISTRIBUTION_H
#define TDISTRIBUTION_H

#include "common.hh"

using namespace _Goptical::Trace;

class TDistribution
{
public:
    /** Creates a distribution pattern with specified pattern,
      radial ray density and scaling.

      The scaling ratio parameter may be used to avoid
      distributing rays too close to the surface edge. */
    TDistribution(Pattern pattern = DefaultDist,
                        unsigned int radial_density = 5,
                        double scaling = 0.999);

    /** Set distribution pattern */
    void set_pattern(Pattern p);

    /** Get distribution pattern */
    Pattern get_pattern() const;

    /** Set average number of ray to distribute on surface radius */
    void set_radial_density(unsigned int count);

    /** Get current radial density */
    unsigned int get_radial_density() const;

    /** Set scaling, Rays are distributed on surface but are
      kept away from surface edge if scaling is less than
      1.0. */
    void set_scaling(double scaling);

    /** get current scaling */
    double get_scaling() const;

    /** Ensure uniform 2d pattern suitable for spot diagram and
      image analysis is selected. Change to default pattern if
      not. */
    void set_uniform_pattern();

private:
    Pattern           _pattern;
    unsigned int      _radial_density;
    double            _scaling;
};

#endif // TDISTRIBUTION_H
