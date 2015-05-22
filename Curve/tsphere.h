#ifndef TSPHERE_H
#define TSPHERE_H

#include "tconicbase.h"

class TSphere : public TConicBase
{
public:
    /** Creates a spherical curve with given radius of curvature */
    TSphere(double roc);

    bool intersect(Math::Vector3 &point, const Math::VectorPair3 &ray) const;
    void normal(Math::Vector3 &normal, const Math::Vector3 &point) const;

    double sagitta(double r) const;
    double derivative(double r) const;
};

#endif // TSPHERE_H
