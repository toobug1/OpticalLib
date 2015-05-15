#ifndef TTRIANGLE_H
#define TTRIANGLE_H

#include "tvector3d.h"


class TTriangle
{
public:
    TTriangle();
    TTriangle(const TVector3D &a, const TVector3D &b, const TVector3D &c);


    const TVector3D & operator[](int n) const;
    TVector3D & operator[](int n);

    TVector3D get_centroid() const;
    TVector3D normal() const;

protected:
    TVector3D _v[3];

};

#endif // TTRIANGLE_H
