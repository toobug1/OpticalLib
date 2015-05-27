#ifndef TSOURCERAYS_H
#define TSOURCERAYS_H

#include "tsource.h"
#include "tsurface.h"
#include "Light/tray.h"
#include "vector_pool"
#include <map>

class TSourceRays : public TSource
{
public:
    /** Create a rays source. Source object position is used by some
      of the @tt add_* functions and may be specified. */
    TSourceRays(const Math::Vector3 &object = Math::vector3_0);

    /** Add chief rays to system entrance pupil for all defined wavelengths. */
    void add_chief_rays(const TSystem &sys);
    /** Add chief rays to specified surface for all defined wavelengths. */
    void add_chief_rays(const TSurface &s);

    /** Add marginal rays to system entrance pupil for all defined
      wavelengths. @see __add_marginal_rays2__ */
    void add_marginal_rays(const TSystem &sys, double entrance_height = 0.);

    /** Add marginal rays to specified surface for all defined wavelengths.
   @pb Source direction (rotation) is used to determine tangential plane.
   @pb Entrance height is set to surface aperture radius when 0.
   @alias add_marginal_rays2 */
    void add_marginal_rays(const TSurface &s, double entrance_height = 0.);

    /** Creates light rays from position and direction for all
      defined wavelengths. @pb Ray origin and direction are
      expressed in specified @tt ref element coordinates or global
      coordinates if none is specified. */
    void add_rays(const Math::VectorPair3 &ray, const TElement *ref = 0);

    /** Add a single light ray; rays properties are copied from
      specified light ray. @pb Ray origin and direction are
      expressed in specified @tt ref element coordinates or global
      coordinates if none is specified.*/
    void add_ray(const TRay &ray, const TElement *ref = 0);

    /** Discard all defined rays  */
    void clear_rays();

private:

    void generate_rays_simple(TResult &result,
                              const targets_t &entry) const;

    void generate_rays_intensity(TResult &result,
                                 const targets_t &entry) const;

    void wavelen_ref_inc(double wl);
    void wavelen_ref_drop(double wl);

    /* storage for first 32 rays */
    vector_pool<TRay, 32>::block_type _rays_storage;
    /* rays allocation pool */
    vector_pool<TRay, 32>       _rays;
    /* wavelen use count map */
    typedef std::map<double, size_t> wl_map_t;
    wl_map_t          _wl_map;
};

#endif // TSOURCERAYS_H
