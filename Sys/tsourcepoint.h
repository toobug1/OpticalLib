#ifndef TSOURCEPOINT_H
#define TSOURCEPOINT_H

#include "tsource.h"

using namespace _Goptical::Sys
;
class TSourcePoint : public TSource
{
public:
    /** Create a point source with given mode. A direction vector
      must be provided when source is in infinity mode. If not in
      infinity mode, a position vector must be provided
      instead. */
    TSourcePoint(SourceInfinityMode m, const Math::Vector3 &pos_dir);

    /** Set point source mode to infinity and adjust source direction vector */
    void set_infinity_direction(const Math::Vector3 &dir);

    /** Set point source mode to positioned and adjust source position vector */
    void set_position(const Math::Vector3 &pos);

    /** Change current point source infinity mode */
    void set_mode(SourceInfinityMode mode);

private:

    void generate_rays_simple(TResult &result,
                              const targets_t &entry) const;
    void generate_rays_intensity(Trace::Result &result,
                                 const targets_t &entry) const;

    template <SourceInfinityMode mode>
    void get_lightrays_(Trace::Result &result,
                               const Element &target) const;

    SourceInfinityMode _mode;
};

#endif // TSOURCEPOINT_H
