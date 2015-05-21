#ifndef TCURVEBASE_H
#define TCURVEBASE_H

#include "Math/Vector"
#include "Math/VectorPair"

#include <QPair>


using namespace _Goptical;

class TCurveBase
{
public:
    virtual ~TCurveBase() {}

    /** Get curve sagitta at specified point */
    virtual double sagitta(const Math::Vector2 & xy) const = 0;

    /** Get curve x and y derivative (gradient) at specified point */
    virtual void derivative(const Math::Vector2 & xy, Math::Vector2 & dxdy) const;

    /** Get intersection point between curve and 3d ray. Return
        false if no intersection occurred */
    virtual bool intersect(Math::Vector3 &point, const Math::VectorPair3 &ray) const;

    /** Get normal to curve surface at specified point */
    virtual void normal(Math::Vector3 &normal, const Math::Vector3 &point) const;
};

#endif // TCURVEBASE_H
