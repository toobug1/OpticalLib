#include "tdistribution.h"
#include "error.hh"

using _Goptical::Error;

TDistribution::TDistribution(Pattern pattern,
                             unsigned int radial_density,
                             double scaling)
    : _pattern(pattern),
      _radial_density(radial_density),
      _scaling(scaling)
{
    if (radial_density < 1)
        throw Error("ray distribution radial density must be greater than 1");
}

void TDistribution::set_pattern(Pattern p)
{
    _pattern = p;
}

Pattern TDistribution::get_pattern() const
{
    return _pattern;
}

unsigned int TDistribution::get_radial_density() const
{
    return _radial_density;
}

void TDistribution::set_radial_density(unsigned int density)
{
    _radial_density = density;
}

double TDistribution::get_scaling() const
{
    return _scaling;
}

void TDistribution::set_scaling(double margin)
{
    _scaling = margin;
}

void TDistribution::set_uniform_pattern()
{
    switch (_pattern)
    {
    case SagittalDist:
    case MeridionalDist:
    case CrossDist:
        _pattern = DefaultDist;
    default:
        ;
    }
}

