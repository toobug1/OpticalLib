#include "tvectorpair3d.h"

TVectorPair3D::TVectorPair3D()
{

}

TVectorPair3D::~TVectorPair3D()
{

}

TVector3D& TVectorPair3D::direction()
{
    return second;
}

const TVector3D& TVectorPair3D::direction() const
{
    return second;
}
TVector3D& TVectorPair3D::origin()
{
    return first;
}

const TVector3D& TVectorPair3D::origin() const
{
    return first;
}
TVector3D& TVectorPair3D::normal()
{
    return second;
}

const TVector3D& TVectorPair3D::normal()    const
{
    return second;
}

double TVectorPair3D::pl_ln_intersect_scale(const TVectorPair3D &line) const
{
    return (TVector3D::dotProduct(first, second) - TVector3D::dotProduct(second, line.first)) / TVector3D::dotProduct(line.second, second);
}

