#ifndef TSTOP_H
#define TSTOP_H

#include "tsurface.h"
#include <QSharedPointer>

class TParams;

class TStop : public TSurface
{
public:
    /** Create an aperture stop surface. External radius is set to
      twice the shape max radius. */
    Stop(const Math::VectorPair3 &p, const QSharedPointer<TShapeBase> &shape);

    /** Create a circular aperture stop surface with given
      radius. External radius is set to twice the shape max
      radius. */
    Stop(const Math::VectorPair3 &p, double radius);

    GOPTICAL_ACCESSORS(double, external_radius,
                       "stop external radius. @see Stop")

    GOPTICAL_ACCESSORS(bool, intercept_reemit,
                       "intercept and reemit enabled. @see Stop")

private:

    /** @override */
    void draw_2d_e(Io::Renderer &r, const TElement *ref) const;
    /** @override Only draw stop surface edges */
    void draw_3d_e(Io::Renderer &r, const TElement *ref) const;

    template <Trace::IntensityMode m>
    void process_rays_(TResult &result,
                              rays_queue_t *input) const;

    /** @override */
    bool intersect(const TParams &params,
                   Math::VectorPair3 &pt,
                   const Math::VectorPair3 &ray) const;

    /** @override */
    void trace_ray_simple(TResult &result,
                          TTraceRay &incident,
                          const Math::VectorPair3 &local,
                          const Math::VectorPair3 &intersect) const;

    /** @override */
    void trace_ray_intensity(TResult &result,
                             TTraceRay &incident,
                             const Math::VectorPair3 &local,
                             const Math::VectorPair3 &intersect) const;

    /** @override */
    void process_rays_simple(TResult &result,
                             rays_queue_t *input) const;

    /** @override */
    void process_rays_intensity(TResult &result,
                                rays_queue_t *input) const;

    /** @override */
    void process_rays_polarized(TResult &result,
                                rays_queue_t *input) const;

    double _external_radius;
    bool _intercept_reemit;
};

#endif // TSTOP_H
