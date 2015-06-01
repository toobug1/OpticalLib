#include "tsurface.h"
#include "Math/Vector"
#include "Trace/tresult.h"
#include "Trace/tparams.h"
#include "Io/trenderer.h"
#include "delegate"
#include "Error"

void TSurface::set_curve(const QSharedPointer<TCurveBase> &c)
{
    _curve = c;
}

const QSharedPointer<TCurveBase> & TSurface::get_curve() const
{
    return _curve;
}

void TSurface::set_shape(const QSharedPointer<TShapeBase> &s)
{
    _shape = s;
}

const QSharedPointer<TShapeBase> & TSurface::get_shape() const
{
    return _shape;
}

void TSurface::set_discard_intensity(double intensity)
{
    _discard_intensity = intensity;
}

inline double TSurface::get_discard_intensity() const
{
    return _discard_intensity;
}

TSurface::TSurface(const Math::VectorPair3 &p,
                   const QSharedPointer<TCurveBase> &curve,
                   const QSharedPointer<TShapeBase> &shape)
    : TElement(p),
      _discard_intensity(0),
      _curve(curve),
      _shape(shape)
{
}

TSurface::~TSurface()
{
}

void TSurface::get_pattern(const Math::Vector3::put_delegate_t &f,
                          const TDistribution &d,
                          bool unobstructed) const
{
    DPP_DELEGATE2_OBJ(de, void, (const Math::Vector2 &v2d),
                      const Math::Vector3::put_delegate_t &, f, // _0
                      const QSharedPointer<TCurveBase> &, _curve,    // _1
    {
                          _0(Math::Vector3(v2d, _1->sagitta(v2d)));
                      });

    // get distribution from shape
    _shape->get_pattern(de, d, unobstructed);
}

void TSurface::trace_ray_simple(TResult &result,
                                TTraceRay &incident,
                                const Math::VectorPair3 &local,
                                const Math::VectorPair3 &intersect) const
{
    throw Error("simple ray trace not handled by this TSurface class");
}

void TSurface::trace_ray_intensity(TResult &result,
                                   TTraceRay &incident,
                                   const Math::VectorPair3 &local,
                                   const Math::VectorPair3 &intersect) const
{
    throw Error("intensity ray trace not handled by this TSurface class");
}

void TSurface::trace_ray_polarized(TResult &result,
                                   TTraceRay &incident,
                                   const Math::VectorPair3 &local,
                                   const Math::VectorPair3 &intersect) const
{
    throw Error("polarized ray trace not handled by this TSurface class");
}

bool TSurface::intersect(const TParams &params,
                         Math::VectorPair3 &pt,
                         const Math::VectorPair3 &ray) const
{
    if (!_curve->intersect(pt.origin(), ray))
        return false;

    if (!params.get_unobstructed() &&
            !_shape->inside(pt.origin().project_xy()))
        return false;

    _curve->normal(pt.normal(), pt.origin());
    if (ray.direction().z() < 0)
        pt.normal() = -pt.normal();

    return true;
}

template <Trace::IntensityMode m>
void TSurface::trace_ray(TResult &result,
                         TTraceRay &incident,
                         const Math::VectorPair3 &local,
                         const Math::VectorPair3 &pt) const
{
    incident.set_len((pt.origin() - local.origin()).len());
    incident.set_intercept(*this, pt.origin());

    if (m == Trace::SimpleTrace)
    {
        incident.set_intercept_intensity(1.0);
        return trace_ray_simple(result, incident, local, pt);
    }
    else
    {
        // apply absorbtion from current material
        double i_intensity = incident.get_intensity() *
                incident.get_material()->get_internal_transmittance(
                    incident.get_wavelen(), incident.get_len());

        incident.set_intercept_intensity(i_intensity);

        if (i_intensity < _discard_intensity)
            return;

        if (m == Trace::IntensityTrace)
            return trace_ray_intensity(result, incident, local, pt);
        else if (m == Trace::PolarizedTrace)
            return trace_ray_polarized(result, incident, local, pt);
        else
            abort();
    }
}

template void TSurface::trace_ray<Trace::SimpleTrace>(TResult &result,
                                                      TTraceRay &incident,
                                                      const Math::VectorPair3 &local,
                                                      const Math::VectorPair3 &pt) const;
template void TSurface::trace_ray<Trace::IntensityTrace>(TResult &result, TTraceRay &incident,
const Math::VectorPair3 &local, const Math::VectorPair3 &pt) const;
template void TSurface::trace_ray<Trace::PolarizedTrace>(TResult &result, TTraceRay &incident,
const Math::VectorPair3 &local, const Math::VectorPair3 &pt) const;

template <Trace::IntensityMode m>
inline void TSurface::process_rays_(TResult &result,
                                    rays_queue_t *input) const
{
    const TParams &params = result.get_params();

    GOPTICAL_FOREACH(i, *input)
    {
        Math::VectorPair3 pt;
        TTraceRay  &ray = **i;

        const Math::Transform<3> &t = ray.get_creator()->get_transform_to(*this);
        Math::VectorPair3 local(t.transform_line(ray));

        if (intersect(params, pt, local))
        {
            result.add_intercepted(*this, ray);

            trace_ray<m>(result, ray, local, pt);
        }
    }
}

void TSurface::process_rays_simple(TResult &result,
                                   rays_queue_t *input) const
{
    process_rays_<Trace::SimpleTrace>(result, input);
}

void TSurface::process_rays_intensity(TResult &result,
                                      rays_queue_t *input) const
{
    process_rays_<Trace::IntensityTrace>(result, input);
}

void TSurface::process_rays_polarized(TResult &result,
                                      rays_queue_t *input) const
{
    process_rays_<Trace::PolarizedTrace>(result, input);
}

TRgb TSurface::get_color(const TRenderer &r) const
{
    return r.get_style_color(Io::StyleSurface);
}

Math::VectorPair3 TSurface::get_bounding_box() const
{
    Math::VectorPair2 sb = _shape->get_bounding_box();

    // FIXME we assume curve is symetric here
    double z = 0;
    double ms = _curve->sagitta(Math::Vector2(_shape->max_radius()));

    if (z > ms)
        std::swap(z, ms);

    return Math::VectorPair3(Math::Vector3(sb[0].x(), sb[0].y(), z),
            Math::Vector3(sb[1].x(), sb[1].y(), ms));
}

void TSurface::draw_3d_e(TRenderer &r, const TElement *ref) const
{
    TRgb color = get_color(r);

    std::vector<Math::Triangle<2> >   mesh;
    delegate_push<typeof(mesh)>       mesh_push(mesh);
    _shape->get_triangles(mesh_push, r.get_feature_size());

    const Math::Transform<3> &tr = get_global_transform();

    GOPTICAL_FOREACH(t, mesh)
    {
        Math::Triangle<3> pts, nrm;

        for (unsigned int i = 0; i < 3; i++)
        {
            pts[i].x() = (*t)[i].x();
            pts[i].y() = (*t)[i].y();
            pts[i].z() = _curve->sagitta((*t)[i]);

            _curve->normal(nrm[i], pts[i]);

            pts[i] = tr.transform(pts[i]);
            nrm[i] = tr.transform_linear(nrm[i]);
        }

        r.draw_triangle(pts, nrm, color);
    }
}

void TSurface::get_2d_points(Math::Vector2 *array,
                            unsigned int count,
                            double start, double end,
                            const TElement *ref) const
{
    assert(count > 1);

    double    y1 = start;
    double    step = (end - start) / (count - 1);
    int i;

    const Math::Transform<3> &t = get_transform_to(ref);

    for (i = 0; i < (int)count; i++)
    {
        Math::Vector3 v(0., y1, 0.);
        v.z() = _curve->sagitta(v.project_xy());

        array[i] = t.transform(v).project_zy();
        y1 += step;
    }
}

void TSurface::draw_2d_e(TRenderer &r, const TElement *ref) const
{
    double top_edge = _shape->get_outter_radius(Math::vector2_01);
    double top_hole = _shape->get_hole_radius(Math::vector2_01);

    double bot_edge = -_shape->get_outter_radius(-Math::vector2_01);
    double bot_hole = -_shape->get_hole_radius(-Math::vector2_01);

    unsigned int res = std::max(100, std::min(4, (int)(fabs(top_edge - bot_edge) / r.get_feature_size())));

    if (fabs(bot_hole - top_hole) > 1e-6)
    {
        DPP_VLARRAY(Math::Vector2, res / 2, p);

        get_2d_points(&p[0], res / 2, top_edge, top_hole, ref);
        r.draw_polygon(&p[0], res / 2, get_color(r), false, false);
        get_2d_points(&p[0], res / 2, bot_hole, bot_edge, ref);
        r.draw_polygon(&p[0], res / 2, get_color(r), false, false);
    }
    else
    {
        DPP_VLARRAY(Math::Vector2, res, p);

        get_2d_points(&p[0], res, top_edge, bot_edge, ref);
        r.draw_polygon(&p[0], res, get_color(r), false, false);
    }
}

