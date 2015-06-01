#include "timage.h"
#include "Curve/tflat.h"
#include "Shape/trectangle.h"

TImage::TImage(const Math::VectorPair3 &p,
             const QSharedPointer<TCurveBase> &curve,
             const QSharedPointer<TShapeBase> &shape)
  : TSurface(p, curve, shape)
{
}

TImage::TImage(const Math::VectorPair3 &p, double radius)
  : TSurface(p, QSharedPointer<TFlat>(&flat),
             QSharedPointer<TRectangle>(new TRectangle(radius * 2.0)))
{
}

void TImage::trace_ray_simple(TResult &result,
                              TTraceRay &incident,
                              const Math::VectorPair3 &local,
                              const Math::VectorPair3 &intersect) const
{
}

void TImage::trace_ray_intensity(TResult &result,
                                 TTraceRay &incident,
                                 const Math::VectorPair3 &local,
                                 const Math::VectorPair3 &intersect) const
{
}

void TImage::trace_ray_polarized(TResult &result,
                                 TTraceRay &incident,
                                 const Math::VectorPair3 &local,
                                 const Math::VectorPair3 &intersect) const
{
}

