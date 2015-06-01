#ifndef TOPTICALSURFACE_H
#define TOPTICALSURFACE_H

#include <QSharedPointer>

#include "tsurface.h"
#include "Material/tglass.h"

class TSystem;

class TOpticalSurface : public TSurface
{
public:

    /** Create an optical surface at specified location. */
    TOpticalSurface(const Math::VectorPair3 &p,
                    const QSharedPointer<TCurveBase> &curve,
                    const QSharedPointer<TShapeBase> &shape,
                    const QSharedPointer<TGlass> &left,
                    const QSharedPointer<TGlass> &right);

    /** Create a circular aperture optical surface at specified
      location.
      @param ap_radius circular aperture radius.
  */
    TOpticalSurface(const Math::VectorPair3 &p,
                    const QSharedPointer<TCurveBase> &curve,
                    double ap_radius,
                    const QSharedPointer<TGlass> &left,
                    const QSharedPointer<TGlass> &right);

    /** Create a spherical optical surface with circular aperture at
      specified location.
      @param roc spherical radius of curvature. 0 means flat curve.
      @param ap_radius circular aperture radius.
  */
    TOpticalSurface(const Math::VectorPair3 &p,
                   double roc, double ap_radius,
                   const QSharedPointer<TGlass> &left,
                   const QSharedPointer<TGlass> &right);

    virtual ~TOpticalSurface();

    /** Set surface left or right material */
    void set_material(unsigned index, const QSharedPointer<TGlass> &m);

    /** Get surface left or right material */
    const TGlass & get_material(unsigned id) const;

    /** Get surface natural color from material properties. */
    TRgb get_color(const TRenderer &r) const;

private:

    void trace_ray_simple(TResult &result,
                          TTraceRay &incident,
                          const Math::VectorPair3 &local,
                          const Math::VectorPair3 &intersect) const;

    void trace_ray_intensity(TResult &result,
                             TTraceRay &incident,
                             const Math::VectorPair3 &local,
                             const Math::VectorPair3 &intersect) const;

    /** @override */
    void system_register(TSystem &s);

    /** @override */
    void system_unregister();

    /** @override */
    void print(std::ostream &o) const;

    /** compute refracted ray direction according to fresnel law */
    bool refract(const Math::VectorPair3 &ray,
                 Math::Vector3 &direction,
                 const Math::Vector3 &normal,
                 double refract_index) const;

    /** compute reflected ray direction according to fresnel law */
    void reflect(const Math::VectorPair3 &ray,
                 Math::Vector3 &direction,
                 const Math::Vector3 &normal) const;

    QSharedPointer<TGlass>     _mat[2];
};

#endif // TOPTICALSURFACE_H
