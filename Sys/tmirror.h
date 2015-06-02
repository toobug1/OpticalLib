#ifndef TMIRROR_H
#define TMIRROR_H

#include "topticalsurface.h"

class TMirror : public TOpticalSurface
{
public:
#if 0
    /**
     Specifies direction of incoming light rays that will be reflected.
   */
    enum Direction
    {
        /** The mirror will reflect light with negative Z direction
          in its local coordinates */
        LightFromRight = 0,
        /** The mirror will reflect light with positive Z direction
          in its local coordinates */
        LightFromLeft = 1,
    };
#endif

    /** Create a mirror at given position with given shape and
      curve. Default is to reflect light coming from left using
      @ref Material::mirror material. */
    TMirror(const Math::VectorPair3 &p,
            const QSharedPointer<TCurveBase> &curve,
            const QSharedPointer<TShapeBase> &shape,
            bool light_from_left,
            const QSharedPointer<TGlass> &metal,
            const QSharedPointer<TGlass> &env = QSharedPointer<const TGlass>(&none));

    /** Create a circular aperture mirror at given position with
      given curve. Default is to reflect light coming from left
      using @ref Material::mirror material. */
    TMirror(const Math::VectorPair3 &p,
            const QSharedPointer<TCurveBase> &curve,
            double ap_radius,
            bool light_from_left,
            const QSharedPointer<TGlass> &metal,
            const QSharedPointer<TGlass> &env = QSharedPointer<const TGlass>(&none));

    /** Create a conic mirror with circular aperture at given
      position. Default is to reflect light coming from left using @ref
      Material::mirror material. */
    TMirror(const Math::VectorPair3 &p,
            double roc, double sc,
            double ap_radius,
            bool light_from_left,
            const QSharedPointer<TGlass> &metal,
            const QSharedPointer<TGlass> &env = QSharedPointer<const TGlass>(&none));
};

#endif // TMIRROR_H
