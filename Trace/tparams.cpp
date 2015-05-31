#include "tparams.h"
#include "Sys/tsurface.h"

TParams::TParams()
    : _default_distribution(),
      _s_distribution(),
      _max_bounce(50),
      _intensity_mode(SimpleTrace),
      _sequential_mode(false),
      _propagation_mode(RayPropagation),
      _unobstructed(false),
      _lost_ray_length(1000)
{
}

void TParams::set_nonsequential_mode()
{
    _sequential_mode = false;
}

void TParams::set_sequential_mode(const QSharedPointer<TSequence> &seq)
{
    _sequential_mode = true;
    _sequence = seq;
}

bool TParams::is_sequential() const
{
    return _sequential_mode;
}

void TParams::set_distribution(const TSurface &s,
                               const TDistribution &dist)
{
    _s_distribution[&s] = dist;
}

void TParams::reset_distribution()
{
    _s_distribution.clear();
}

const TDistribution & TParams::get_distribution(const TSurface &s) const
{
    _s_distribution_map_t::const_iterator i = _s_distribution.find(&s);

    return i == _s_distribution.end() ? _default_distribution : i->second;
}



