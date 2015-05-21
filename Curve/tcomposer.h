#ifndef TCOMPOSER_H
#define TCOMPOSER_H

#include <QList>
#include <QSharedPointer>

#include "tcurvebase.h"
#include "Math/Transform"
#include "Math/Vector"

using namespace _Goptical;

/**
       @short Enable definition of curve as composition of other curves
       @header Goptical/Curve/Composer
       @module {Core}
       @main

       This class allows definition of a surface curve using the sum
       of several base curves. Each base curve involved can be
       translated, rotated and scaled individually.
    */
class TComposer : public TCurveBase
{
public:

    /**
         @short Curve::Composer base curve attributes.
         @header Goptical/Curve/Composer

         This class enables access to Composer base curve
         transformations. Sagitta (z) scaling and (x, y) affine
         transforms (translation, rotation, scaling) can be applied as
         needed in any order.
       */
    class TAttributes
    {
        friend class TComposer;

    public:
        TAttributes(TCurveBase& base);
        /** Apply sagitta (z) offset. default is 0 */
        TAttributes & z_offset(double zoffset);
        /** Apply sagitta (z) scale factor. default is 1 */
        TAttributes & z_scale(double zfactor);
        /** Apply scaling affine transform using scale factors (xscale, yscale) */
        TAttributes & xy_scale(const Math::Vector2 &factor);
        /** Apply rotation affine transform. Angle is in degree. */
        TAttributes & rotate(double dangle);
        /** Apply translation transform */
        TAttributes & xy_translate(const Math::Vector2 &offset);

    private:
        QSharedPointer<TCurveBase> _curve;
        Math::Transform<2> _transform;
        Math::Transform<2> _inv_transform;

        double _z_scale;
        double _z_offset;
    };

    TComposer();

    /** Add a base curve to use for composition. The returned
          Composer::Attributes object may be used to apply base curve
          transformations. */
    TAttributes & add_curve(TCurveBase &curve);

    double sagitta(const Math::Vector2 & xy) const;
    void derivative(const Math::Vector2 & xy, Math::Vector2 & dxdy) const;

private:
    QList<TAttributes> _list;
};



#endif // TCOMPOSER_H
