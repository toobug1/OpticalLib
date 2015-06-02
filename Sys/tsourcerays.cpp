#include "tsourcerays.h"
#include "tsurface.h"
#include "tsystem.h"
#include "Math/Vector"
#include "telement.h"
#include "Light/tray.h"
#include "Trace/tresult.h"

TSourceRays::TSourceRays(const Math::Vector3 &object)
    : TSource(object),
      _rays(_rays_storage)
{
}

void TSourceRays::wavelen_ref_inc(double wl)
{
    wl_map_t::iterator i = _wl_map.insert(
                wl_map_t::value_type(wl, 0)).first;
    i->second++;
}

void TSourceRays::wavelen_ref_drop(double wl)
{
    wl_map_t::iterator i = _wl_map.find(wl);
    assert(i != _wl_map.end());
    if (!--i->second)
        _wl_map.erase(i);
}

void TSourceRays::add_chief_rays(const TSystem &sys)
{
    add_chief_rays(sys.get_entrance_pupil());
}

void TSourceRays::add_chief_rays(const TSurface &s)
{
    const Math::Vector3 so = s.get_position(*this);

    // ray in source coordinates
    const Math::VectorPair3 r(Math::vector3_0, so.normalized());

    GOPTICAL_FOREACH(l, _spectrum)
            _rays.create(r, l->get_intensity(), l->get_wavelen());
}

void TSourceRays::add_marginal_rays(const TSystem &sys, double entrance_height)
{
    add_marginal_rays(sys.get_entrance_pupil(), entrance_height);
}

void TSourceRays::add_marginal_rays(const TSurface &s, double entrance_height)
{
    const Math::Transform<3> &t1 = s.get_transform_to(*this);
    const Math::Transform<3> &t2 = get_transform_to(s);

    // get source up vector direction in surface coordinates
    const Math::Vector2 up = t2.transform_linear(Math::vector3_010).project_xy().normalized();

    // get surface edge point in that direction
    if (entrance_height == 0.)
        entrance_height = s.get_shape()->get_outter_radius(up);

    const Math::Vector2 edge2 = up * (entrance_height - 1e-8);
    // account for surface curvature to get z value
    const Math::Vector3 edge3(edge2, s.get_curve()->sagitta(edge2));

    // get source point projection on surface axis
    const Math::Vector3 on_axis(0., 0., t2.transform(Math::vector3_0).z());

    // ray in source coordinates
    const Math::VectorPair3 r(t1.transform_line(Math::VectorPair3(on_axis,
                                                                  (edge3 - on_axis).normalized())));

    GOPTICAL_FOREACH(l, _spectrum)
            _rays.create(r, l->get_intensity(), l->get_wavelen());
}

void TSourceRays::add_rays(const Math::VectorPair3 &ray, const TElement *ref)
{
    GOPTICAL_FOREACH(l, _spectrum)
    {
        TRay r(ray, l->get_intensity(), l->get_wavelen());
        add_ray(r, ref);
    }
}

void TSourceRays::add_ray(const TRay &ray, const TElement *ref)
{
    TRay &r = _rays.create(ray);
    wavelen_ref_inc(ray.get_wavelen());

    r = ray;

    if (ref != this)
    {
        if (ref)
            r = ref->get_transform_to(*this).transform_line(ray);
        else
            r = ref->get_local_transform().transform_line(ray);
    }
}

void TSourceRays::clear_rays()
{
    _rays.clear();
    _wl_map.clear();
}

void TSourceRays::generate_rays_simple(TResult &result,
                                       const targets_t &entry) const
{
    const TGlass *m = !_mat.isNull()
            ? _mat.data() : &get_system()->get_environment_proxy();

    GOPTICAL_FOREACH(w, _wl_map)
            if (w->second)
            result.add_ray_wavelen(w->first);

    GOPTICAL_FOREACH(lr, _rays)
    {
        TTraceRay &r = result.new_ray(*lr);

        r.set_creator(this);
        r.set_material(m);
    }
}

void TSourceRays::generate_rays_intensity(TResult &result,
                                         const targets_t &entry) const
{
    generate_rays_simple(result, entry);
}

