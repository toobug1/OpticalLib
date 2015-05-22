#ifndef TPARABOLA_H
#define TPARABOLA_H

#include "tconicbase.h"


class TParabola : public TConicBase
{
public:
    /** Creates a parabola curve with given radius of curvature */
    TParabola(double roc);

    bool intersect(Math::Vector3 &point, const Math::VectorPair3 &ray) const;

    double sagitta(double r) const;
    double derivative(double r) const;
};

#endif // TPARABOLA_H
