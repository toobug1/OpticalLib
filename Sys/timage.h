#ifndef TIMAGE_H
#define TIMAGE_H

#include "tsurface.h"
#include "Math/VectorPair"

class TImage : public TSurface
{
public:

    /** Create a new image plane at given position. */
    TImage(const Math::VectorPair3 &position,
          const QSharedPointer<TCurveBase> &curve,
          const QSharedPointer<TShapeBase> &shape);

    /** Create a new flat square image plane at given position with given half width */
    TImage(const Math::VectorPair3 &position, double radius);

private:
    void trace_ray_simple(Trace::Result &result,
                          Trace::Ray &incident,
                          const Math::VectorPair3 &local,
                          const Math::VectorPair3 &intersect) const;
    void trace_ray_intensity(Trace::Result &result,
                             Trace::Ray &incident,
                             const Math::VectorPair3 &local,
                             const Math::VectorPair3 &intersect) const;
    void trace_ray_polarized(Trace::Result &result,
                             Trace::Ray &incident,
                             const Math::VectorPair3 &local,
                             const Math::VectorPair3 &intersect) const;
};

#endif // TIMAGE_H
