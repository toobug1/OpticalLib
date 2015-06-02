#include "tmirrorsurface.h"
#include "Curve/tconic.h"
#include "Curve/tflat.h"
#include "Shape/tdisk.h"

TMirrorSurface::TMirrorSurface(const Math::VectorPair3 &p,
                 const QSharedPointer<TCurveBase> &curve,
                 const QSharedPointer<TShapeBase> &shape,
                 bool light_from_left,
                 const QSharedPointer<TGlass> &metal,
                 const QSharedPointer<TGlass> &air)
  : TOpticalSurface(p, curve, shape,
                   light_from_left ? air : metal,
                   light_from_left ? metal : air)
{
}

TMirrorSurface::TMirrorSurface(const Math::VectorPair3 &p,
                 const QSharedPointer<TCurveBase> &curve,
                 double ap_radius,
                 bool light_from_left,
                 const QSharedPointer<TGlass> &metal,
                 const QSharedPointer<TGlass> &air)
  : TOpticalSurface(p, curve, ap_radius,
                    light_from_left ? air : metal,
                    light_from_left ? metal : air)
{
}

TMirrorSurface::TMirrorSurface(const Math::VectorPair3 &p,
                 double roc, double sc,
                 double ap_radius,
                 bool light_from_left,
                 const QSharedPointer<TGlass> &metal,
                 const QSharedPointer<TGlass> &air)
  : TOpticalSurface(p, roc == 0. ? QSharedPointer<TCurveBase>(&flat) :
                                   QSharedPointer<TCurveBase>(new TConic(roc, sc)),
                   QSharedPointer<TShapeBase>(new TDisk(ap_radius)),
                    light_from_left ? air : metal,
                    light_from_left ? metal : air)
{
}
