#ifndef TSURFACE_H
#define TSURFACE_H

#include <QSharedPointer>

#include "common.hh"
#include "telement.h"
#include "Curve/tcurvebase.h"
#include "Shape/tshapebase.h"
#include "Math/VectorPair"
#include "Io/trgb.h"
#include "Trace/tparams.h"
#include "Trace/ttraceray.h"


class TSurface : public TElement
{
public:
    /** Create a surface element at given location. */
    TSurface(const Math::VectorPair3 &p,
             const QSharedPointer<TCurveBase> &curve,
             const QSharedPointer<TShapeBase> &shape);

    virtual ~TSurface();

    /** Set surface curve */
    void set_curve(const QSharedPointer<TCurveBase> &c);
    /** Get surface curve */
    const QSharedPointer<TCurveBase>& get_curve() const;

    /** Set surface shape */
    void set_shape(const QSharedPointer<TShapeBase> &s);
    /** Get surface shape */
    const QSharedPointer<TShapeBase> & get_shape() const;

    /** Get intersection point and normal to surface at
      intersection. Return false if no intersection occured.
  */
    virtual bool intersect(const TParams &params,
                           Math::VectorPair3 &pt,
                           const Math::VectorPair3 &ray) const;

    /** Get distribution pattern points projected on the surface */
    void get_pattern(const Math::Vector3::put_delegate_t &f,
                     const TDistribution &d,
                     bool unobstructed = false) const;

    /** Trace a single ray through the surface */
    template <Trace::IntensityMode m>
    void trace_ray(TResult &result,
                   TTraceRay &incident,
                   const Math::VectorPair3 &local,
                   const Math::VectorPair3 &intersect) const;

    /** Get surface apparent color */
    virtual TRgb get_color(const TRenderer &r) const;

    /** Set minimal ray intensity. Incident rays with less intensity
      will be discarded */
    void set_discard_intensity(double intensity);
    /** Get minimal ray intensity. */
    double get_discard_intensity() const;

    Math::VectorPair3 get_bounding_box() const;

protected:

    /** This function must be reimplemented by subclasses to handle
      incoming rays and generate new ones when in simple ray trace mode. */
    virtual void trace_ray_simple(TResult &result,
                                  TTraceRay &incident,
                                  const Math::VectorPair3 &local,
                                  const Math::VectorPair3 &intersect) const;

    /** This function must be reimplemented by subclasses to handle
      incoming rays and generate new ones when in intensity ray trace mode. */
    virtual void trace_ray_intensity(TResult &result,
                                     TTraceRay &incident,
                                     const Math::VectorPair3 &local,
                                     const Math::VectorPair3 &intersect) const;

    /** This function must be reimplemented by subclasses to handle
      incoming rays and generate new ones when in polarized ray trace mode. */
    virtual void trace_ray_polarized(TResult &result,
                                     TTraceRay &incident,
                                     const Math::VectorPair3 &local,
                                     const Math::VectorPair3 &intersect) const;

    /** @override */
    void draw_2d_e(TRenderer &r, const TElement *ref) const;
    /** @override */
    void draw_3d_e(TRenderer &r, const TElement *ref) const;

private:

    /** @internal */
    void get_2d_points(Math::Vector2 *array,
                       unsigned int count,
                       double start,
                       double end,
                       const TElement *ref) const;

    template <Trace::IntensityMode m>
    void process_rays_(TResult &result,
                       rays_queue_t *input) const;

    virtual void process_rays_simple(TResult &result,
                                     rays_queue_t *input) const;

    virtual void process_rays_intensity(TResult &result,
                                        rays_queue_t *input) const;

    virtual void process_rays_polarized(TResult &result,
                                        rays_queue_t *input) const;

    double                    _discard_intensity;
    QSharedPointer<TCurveBase>   _curve;
    QSharedPointer<TShapeBase>   _shape;
};

#endif // TSURFACE_H
