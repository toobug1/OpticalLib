#ifndef TPARAMS_H
#define TPARAMS_H

#include <map>
#include <QSharedPointer>

#include "common.hh"
#include "tdistribution.h"
#include "tsequence.h"

class TSurface;

class TParams
{
    friend class TTracer;

public:
    TParams();

    GOPTICAL_NOCONST_REF_ACCESSORS(TDistribution, default_distribution,
                                   "default rays distribution pattern")

    GOPTICAL_ACCESSORS(unsigned int, max_bounce,
                       "maximum ray bounce count, default is 50")

    GOPTICAL_ACCESSORS(double, lost_ray_length,
                       "lost ray length")

    GOPTICAL_ACCESSORS(Trace::IntensityMode, intensity_mode,
                       "raytracing intensity mode")

    GOPTICAL_ACCESSORS(bool, unobstructed,
           "unobstructed raytracing mode. Surface shapes are ignored, no rays are stopped")

    GOPTICAL_ACCESSORS(PropagationMode, propagation_mode,
            "physical light propagation mode. @experimental @hidden")

    /** Set sequential ray tracing mode */
    void set_sequential_mode(const QSharedPointer<TSequence> &seq);

    /** Set non sequential ray tracing mode (default) */
    void set_nonsequential_mode();

    /** Test if in sequential ray tracing mode */
    bool is_sequential() const;

    /** Set distribution pattern for a given surface */
    void set_distribution(const TSurface &s,
                                 const TDistribution &dist);

    /** Reset all surface specific distribution settings to default */
    void reset_distribution();

    /** Get distribution pattern for a given surface */
    const TDistribution & get_distribution(const TSurface &s) const;

private:
    typedef std::map<const TSurface *, TDistribution> _s_distribution_map_t;

    QSharedPointer<TSequence>       _sequence;
    TDistribution              _default_distribution;
    _s_distribution_map_t     _s_distribution;
    unsigned int              _max_bounce;
    Trace::IntensityMode             _intensity_mode;
    bool                      _sequential_mode;
    PropagationMode           _propagation_mode;
    bool                      _unobstructed;
    double                    _lost_ray_length;
};

#endif // TPARAMS_H
