#ifndef TVECTORPAIR3D_H
#define TVECTORPAIR3D_H

#include "Math/tvector3d.h"
#include <QPair>

class TVectorPair3D : public QPair<TVector3D, TVector3D>
{
public:
    TVectorPair3D();
    TVectorPair3D(const TVector3D& fst, const TVector3D& secd) {first = fst; second = secd;}
    ~TVectorPair3D();

          TVector3D& direction();
    const TVector3D& direction() const;
          TVector3D& origin();
    const TVector3D& origin()    const;
          TVector3D& normal();
    const TVector3D& normal()    const;

    double pl_ln_intersect_scale(const TVectorPair3D &line) const;
};

#endif // TVECTORPAIR3D_H
