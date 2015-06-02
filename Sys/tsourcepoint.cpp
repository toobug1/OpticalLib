
#define DPP_DELEGATE_ARGC 5

#include "tsourcepoint.h"
#include "Trace/tdistribution.h"
#include "tsurface.h"
#include "Trace/tresult.h"
#include "Math/Vector"

#include <limits>

using namespace dpp;

TSourcePoint::TSourcePoint(SourceInfinityMode m, const Math::Vector3 &pos_dir)
    : TSource(m == SourceAtInfinity
              // position of infinity source is only used for Trace::Sequence sort
              ? Math::VectorPair3(pos_dir * -1e9, pos_dir)
              : Math::VectorPair3(pos_dir, Math::vector3_001)),
      _mode(m)
{
}



void TSourcePoint::generate_rays_simple(TResult &result,
                                        const targets_t &entry) const
{
    GOPTICAL_FOREACH(l, _spectrum)
            result.add_ray_wavelen(l->get_wavelen());

    switch (_mode)
    {
    case SourceAtFiniteDistance:
        GOPTICAL_FOREACH(target, entry)
                get_lightrays_<SourceAtFiniteDistance>(result, **target);
        return;

    case SourceAtInfinity:
        GOPTICAL_FOREACH(target, entry)
                get_lightrays_<SourceAtInfinity>(result, **target);
        return;
    }
}

void TSourcePoint::generate_rays_intensity(TResult &result,
                                           const targets_t &entry) const
{
    generate_rays_simple(result, entry);
}


void TSourcePoint::set_mode(SourceInfinityMode mode)
{
    _mode = mode;
}

template <SourceInfinityMode mode>
inline void TSourcePoint::get_lightrays_(TResult &result,
                                         const TElement &target) const
{
    const TSurface *starget = dynamic_cast<const TSurface*>(&target);

    if (!starget)
        return;

    double rlen = result.get_params().get_lost_ray_length();

    const TDistribution &d = result.get_params().get_distribution(*starget);

    DPP_DELEGATE5_OBJ(de, void, (const Math::Vector3 &i),

                      // _0
                      const TSourcePoint *, this,

                      // _1 ray aiming at target surface origin in source coordinates
                      const Math::VectorPair3 &,
                      Math::VectorPair3(starget->get_position(*this) -
                                        Math::vector3_001 * rlen, Math::vector3_001),

                      // _2 transform from target to source coordinates
                      const Math::Transform<3> &,
                      starget->get_transform_to(*this),

                      // _3
                      const TGlass *,
                      !_mat.isNull() ? _mat.data() : &get_system()->get_environment_proxy(),

                      // _4
                      TResult &, result,

    {
        Math::Vector3 r = _2.transform(i);  // pattern point on target surface
        Math::Vector3 direction;
        Math::Vector3 position;

                          switch (mode)
                          {
                              case (SourceAtFiniteDistance):
                              position = Math::vector3_0;
                              direction = r.normalized();
                              break;

                              case (SourceAtInfinity):
                              direction = Math::vector3_001;
                              position = _1.pl_ln_intersect(Math::VectorPair3(r, direction));
                              break;
                          }

                          GOPTICAL_FOREACH(l, _0->_spectrum)
                          {
                              TTraceRay &r = _4.new_ray();

                              // generated rays use source coordinates
                              r.direction() = direction;
                              r.origin() = position;

                              r.set_creator(_0);
                              r.set_intensity(l->get_intensity()); // FIXME depends on distance from source and pattern density
                              r.set_wavelen(l->get_wavelen());
                              r.set_material(_3);
                          }
     } );

    starget->get_pattern(de, d, result.get_params().get_unobstructed());
}

