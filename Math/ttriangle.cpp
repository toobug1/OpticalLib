#include "ttriangle.h"

TTriangle::TTriangle()
{

}

TTriangle::TTriangle(const TVector3D &a, const TVector3D &b, const TVector3D &c)
{
    _v[0] = a;
    _v[1] = b;
    _v[2] = c;
}

const TVector3D & TTriangle::operator[](int n) const
{
    Q_ASSERT(n < 3);
    return _v[n];
}

TVector3D & TTriangle::operator[](int n)
{
    Q_ASSERT(n < 3);
    return _v[n];
}

TVector3D TTriangle::get_centroid() const
{
    return (_v[0] + _v[1] + _v[2]) / 3;
}

TVector3D TTriangle::normal() const
{
   TVector3D vec = TVector3D::normal(_v[0], _v[1], _v[2]);
   return vec;
}



