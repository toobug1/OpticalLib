#ifndef TCURVEBASE_H
#define TCURVEBASE_H

#include "Math/tvector2d.h"
#include "Math/tvector3d.h"
#include "Math/tvectorpair3d.h"

#include <QPair>

class TCurveBase
{
public:
    TCurveBase();
    virtual ~TCurveBase();

    virtual double sagitta(const TVector2D & xy) const = 0;

    /** Get curve x and y derivative (gradient) at specified point */
    virtual void derivative(const TVector2D & xy, TVector2D & dxdy) const;

    /** Get intersection point between curve and 3d ray. Return
        false if no intersection occurred */

    virtual bool intersect(TVector3D &point, const TVectorPair3D &ray) const;

    /** Get normal to curve surface at specified point */
    virtual void normal(TVector3D &normal, const TVector3D &point) const;
};

#endif // TCURVEBASE_H
