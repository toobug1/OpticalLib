#include "tstop.h"
#include "Curve/tflat.h"
#include "Shape/tdisk.h"
#include "Trace/tparams.h"
#include "Io/trenderer.h"

TStop::TStop(const Math::VectorPair3 &p,
             const QSharedPointer<TShapeBase> &shape)
    : TSurface(p, QSharedPointer<TCurveBase>(&flat), shape),
      _intercept_reemit(false)
{
    _external_radius = shape->max_radius() * 2.0;
}

TStop::TStop(const Math::VectorPair3 &p, double r)
    : TSurface(p, QSharedPointer<TCurveBase>(&flat),
               QSharedPointer<TDisk>(new TDisk(r))),
      _intercept_reemit(false)
{
    _external_radius = r * 2.0;
}

bool TStop::intersect(const TParams &params,
                     Math::VectorPair3 &intersect,
                     const Math::VectorPair3 &ray) const
{
    if (!get_curve()->intersect(intersect.origin(), ray))
        return false;

    Math::Vector2 v(intersect.origin().project_xy());

    if (v.len() > _external_radius)
        return false;

    bool ir = _intercept_reemit || params.is_sequential();

    if (!ir && get_shape()->inside(v))
        return false;

    get_curve()->normal(intersect.normal(), intersect.origin());
    if (ray.direction().z() < 0)
        intersect.normal() = -intersect.normal();

    return true;
}

void TStop::trace_ray_simple(TResult &result,
                                    TTraceRay &incident,
                                    const Math::VectorPair3 &local,
                                    const Math::VectorPair3 &intersect) const
{
    Math::Vector2 v = intersect.origin().project_xy();

    bool ir = _intercept_reemit || result.get_params().is_sequential();

    if (ir && get_shape()->inside(v))
    {
        // reemit incident ray
        TTraceRay &r = result.new_ray();

        r.set_wavelen(incident.get_wavelen());
        r.set_intensity(incident.get_intensity());
        r.set_material(incident.get_material());
        r.origin() = intersect.origin();
        r.direction() = incident.direction();
        r.set_creator(this);

        incident.add_generated(&r);
    }
}

void TStop::trace_ray_intensity(TResult &result,
                                TTraceRay &incident,
                                const Math::VectorPair3 &local,
                                const Math::VectorPair3 &intersect) const
{
    trace_ray_simple(result, incident, local, intersect);
}



void TStop::process_rays_simple(TResult &result,
                                rays_queue_t *input) const
{
    process_rays_<Trace::SimpleTrace>(result, input);
}

void TStop::process_rays_intensity(TResult &result,
                                   rays_queue_t *input) const
{
    process_rays_<Trace::IntensityTrace>(result, input);
}

void TStop::process_rays_polarized(TResult &result,
                                   rays_queue_t *input) const
{
    process_rays_<Trace::PolarizedTrace>(result, input);
}

void TStop::draw_3d_e(TRenderer &r, const TElement *ref) const
{
    r.group_begin();
    std::vector<Math::Vector3 > poly;

    DPP_DELEGATE2_OBJ(d, void, (const Math::Vector2 &t),
                      std::vector<Math::Vector3 > &, poly, // _0
                      const Math::Transform<3> &, get_transform_to(ref), // _1
    {
                          _0.push_back(_1.transform(Math::Vector3(t, 0.)));
                      });

    for (unsigned int i = 0; i < get_shape()->get_contour_count(); i++)
    {
        get_shape()->get_contour(i, d, r.get_feature_size());
        r.draw_polygon(&poly[0], poly.size(), get_color(r));
        poly.clear();
    }

    r.group_end();
}

void TStop::draw_2d_e(TRenderer &r, const TElement *ref) const
{
    Math::Vector3 mr(0, _external_radius, 0);
    Math::Vector3 top(0, get_shape()->get_outter_radius(Math::vector2_01), 0);
    Math::Vector3 bot(0, -get_shape()->get_outter_radius(-Math::vector2_01), 0);

    const Math::Transform<3> t(get_transform_to(ref));

    r.group_begin();
    r.draw_segment(t.transform(top), t.transform(mr), get_color(r));
    r.draw_segment(t.transform(bot), t.transform(-mr), get_color(r));
    r.group_end();
}

