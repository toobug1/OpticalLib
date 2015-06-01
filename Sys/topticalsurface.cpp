#include "topticalsurface.h"
#include "Curve/tsphere.h"
#include "Shape/tdisk.h"
#include "Io/trgb.h"
#include "Curve/tflat.h"
#include "Io/trenderer.h"
#include "Trace/ttraceray.h"
#include "Trace/tresult.h"
#include "tsystem.h"
#include "Material/tdielectricbase.h"
#include "Material/tmaterialbase.h"

TOpticalSurface::TOpticalSurface(const Math::VectorPair3 &p,
                                 const QSharedPointer<TCurveBase> &curve,
                                 const QSharedPointer<TShapeBase> &shape,
                                 const QSharedPointer<TGlass> &left,
                                 const QSharedPointer<TGlass> &right)
    : TSurface(p, curve, shape)
{
    _mat[0] = left;
    _mat[1] = right;
}

TOpticalSurface::TOpticalSurface(const Math::VectorPair3 &p,
                                 double roc, double ap_radius,
                                 const QSharedPointer<TGlass> &left,
                                 const QSharedPointer<TGlass> &right)
    : TSurface(p, roc == 0. ? QSharedPointer<TCurveBase>(&flat) :
                              QSharedPointer<TCurveBase>(new TSphere(roc)),
              QSharedPointer<TShapeBase>(new TDisk(ap_radius)))
{
    _mat[0] = left;
    _mat[1] = right;
}

TOpticalSurface::TOpticalSurface(const Math::VectorPair3 &p,
                                 const QSharedPointer<TCurveBase> &curve,
                                 double ap_radius,
                                 const QSharedPointer<TGlass> &left,
                                 const QSharedPointer<TGlass> &right)
    : TSurface(p, curve, QSharedPointer<TShapeBase>(new TDisk(ap_radius)))
{
    _mat[0] = left;
    _mat[1] = right;
}

TOpticalSurface::~TOpticalSurface()
{
}

TRgb TOpticalSurface::get_color(const TRenderer &r) const
{
    // FIXME color depends on materials
    return r.get_style_color(Io::StyleSurface);
}

bool TOpticalSurface::refract(const Math::VectorPair3 &ray,
                              Math::Vector3 &dir,
                              const Math::Vector3 &normal,
                              double refract_index) const
{
    // Algorithm from Bram de Greve article "Reflections & Refractions in Raytracing"
    // http://www.bramz.org/

    assert(fabs(normal.len() - 1.0) < 1e-10);
    assert(fabs((ray.direction().len()) - 1.0) < 1e-10);

    double cosi = (normal * ray.direction());
    double sint2 = Math::square(refract_index) *
                            (1.0 - Math::square(cosi));

    if (sint2 > 1.0)
        return false;   // total internal reflection

    dir = ray.direction() * refract_index - normal *
            (refract_index * cosi + sqrt(1.0 - sint2));

    return true;
}

void TOpticalSurface::reflect(const Math::VectorPair3 &ray,
                              Math::Vector3 &dir,
                              const Math::Vector3 &normal) const
{
    // Algorithm from Bram de Greve article "Reflections & Refractions in Raytracing"
    // http://www.bramz.org/

    assert(fabs(normal.len() - 1.0) < 1e-10);
    assert(fabs((ray.direction().len()) - 1.0) < 1e-10);

    double    cosi = (normal * ray.direction());

    dir = ray.direction() - normal * (2.0 * cosi);
}

void TOpticalSurface::trace_ray_simple(TResult &result,
                                       TTraceRay &incident,
                                       const Math::VectorPair3 &local,
                                       const Math::VectorPair3 &intersect) const
{
    Math::Vector3     direction;      // refracted ray direction

    bool right_to_left = intersect.normal().z() > 0;

    const TGlass *prev_mat = _mat[right_to_left].data();
    const TGlass *next_mat = _mat[!right_to_left].data();

    // check ray didn't "escaped" from its material
    if (prev_mat != incident.get_material())
        return;

    double wl = incident.get_wavelen();
    double index = prev_mat->getRefractiveIndex(wl) / next_mat->getRefractiveIndex(wl);

    if (!refract(local, direction, intersect.normal(), index))
    {
        TTraceRay &r = result.new_ray();
        // total internal reflection

        r.set_wavelen(wl);
        r.set_intensity(incident.get_intensity());
        r.set_material(prev_mat);
        r.origin() = intersect.origin();
        reflect(local, r.direction(), intersect.normal());
        r.set_creator(this);
        incident.add_generated(&r);

        return;
    }

    // transmit
    if (!next_mat->getGlassPtr()->is_opaque(wl))
    {
        TTraceRay &r = result.new_ray();

        r.set_wavelen(wl);
        r.set_intensity(incident.get_intensity());
        r.set_material(next_mat);
        r.origin() = intersect.origin();
        r.direction() = direction;
        r.set_creator(this);
        incident.add_generated(&r);
    }

    // reflect
    if (next_mat->getGlassPtr()->is_reflecting(wl))
    {
        TTraceRay &r = result.new_ray();

        r.set_wavelen(wl);
        r.set_intensity(incident.get_intensity());
        r.set_material(prev_mat);
        r.origin() = intersect.origin();
        reflect(local, r.direction(), intersect.normal());
        r.set_creator(this);
        incident.add_generated(&r);
    }
}

void TOpticalSurface::trace_ray_intensity(TResult &result,
                                          TTraceRay &incident,
                                          const Math::VectorPair3 &local,
                                          const Math::VectorPair3 &intersect) const
{
    Math::Vector3     direction;

    bool right_to_left = intersect.normal().z() > 0;

    const TGlass *prev_mat = _mat[right_to_left].data();
    const TGlass *next_mat = _mat[!right_to_left].data();

    // check ray didn't "escaped" from its material
    if (prev_mat != incident.get_material())
        return;

    double wl = incident.get_wavelen();
    double index = prev_mat->getRefractiveIndex(wl) / next_mat->getRefractiveIndex(wl);
    double intensity = incident.get_intercept_intensity();

    if (!refract(local, direction, intersect.normal(), index))
    {
        // total internal reflection
        TTraceRay &r = result.new_ray();

        r.set_wavelen(wl);
        r.set_intensity(intensity);
        r.set_material(prev_mat);
        r.origin() = intersect.origin();
        reflect(local, r.direction(), intersect.normal());
        r.set_creator(this);
        incident.add_generated(&r);

        return;
    }

    // transmit
    if (!next_mat->getGlassPtr()->is_opaque(wl))
    {
        double tintensity = intensity * next_mat->getGlassPtr()->
                get_normal_transmittance((TMaterialBase*)prev_mat, wl);

        if (tintensity >= get_discard_intensity())
        {
            TTraceRay &r = result.new_ray();

            r.set_wavelen(wl);
            r.set_intensity(tintensity);
            r.set_material(next_mat);
            r.origin() = intersect.origin();
            r.direction() = direction;
            r.set_creator(this);
            incident.add_generated(&r);
        }
    }

    // reflect
    {
        double rintensity = intensity * next_mat->getGlassPtr()->
                get_normal_reflectance((TMaterialBase*)prev_mat, wl);

        if (rintensity >= get_discard_intensity())
        {
            TTraceRay &r = result.new_ray();

            r.set_wavelen(wl);
            r.set_intensity(rintensity);
            r.set_material(prev_mat);
            r.origin() = intersect.origin();
            reflect(local, r.direction(), intersect.normal());
            r.set_creator(this);
            incident.add_generated(&r);
        }
    }

}

void TOpticalSurface::set_material(unsigned index,
                                   const QSharedPointer<TGlass> &m)
{
    assert(index < 2);

    if (m.isNull() && get_system())
    {
        TGlass* g = new TGlass(get_system()->get_environment_proxy());
        _mat[index].reset(g);
    }
    else
        _mat[index] = m;
}

void TOpticalSurface::system_register(TSystem &s)
{
    TSurface::system_register(s);

    for (unsigned int i = 0; i < 2; i++)
        if (_mat[i].isNull())
        {
            TGlass* g = new TGlass(s.get_environment_proxy());
            _mat[i].reset(g);
        }
}

void TOpticalSurface::system_unregister()
{
    for (unsigned int i = 0; i < 2; i++)
        if (_mat[i].data() == &get_system()->get_environment_proxy())
            _mat[i].clear();

    TSurface::system_unregister();
}

void TOpticalSurface::print(std::ostream &o) const
{
    o << " [" << id() << "]" << typeid(*this).name() << " at " << get_position() << " "
      << typeid(get_material(0)).name() << " " << typeid(get_material(1)).name();
}

const TGlass & TOpticalSurface::get_material(unsigned index) const
{
    assert(index < 2);
    return *_mat[index];
}

