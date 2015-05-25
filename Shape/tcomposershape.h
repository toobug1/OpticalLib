#ifndef TCOMPOSERShape_H
#define TCOMPOSERShape_H

#include <QSharedPointer>

#include "Math/Transform"
#include "Math/Vector"
#include "tshapebase.h"

class TComposerShape : public TShapeBase
{
public:
    class Attributes;

    /** @override */
    bool inside(const Math::Vector2 &point) const;
    /** @override */
    double max_radius() const;
    /** @override */
    double min_radius() const;
    /** @override */
    double get_outter_radius(const Math::Vector2 &dir) const;
    /** @override */
    double get_hole_radius(const Math::Vector2 &dir) const;
    /** @override */
    void get_pattern(const Math::Vector2::put_delegate_t  &f, const TDistribution &d, bool unobstructed) const;
    /** @override */
    Math::VectorPair2 get_bounding_box() const;
    /** @override */
    unsigned int get_contour_count() const;
    /** @override */
    void get_contour(unsigned int contour, const Math::Vector2::put_delegate_t  &f, double resolution) const;
    /** @override */
    void get_triangles(const Math::Triangle<2>::put_delegate_t  &f, double resolution) const;

    /** Add a new shape to shape composer.

      This function returns a reference to an @ref Attributes
      object which may be modified to set shape transform and
      boolean operations.

      The composed shape is the union between all shapes added
      with this function.
   */
    Attributes & add_shape(const QSharedPointer<TShapeBase> &shape);

    /** Set ray distribution behavior. Default is to perform
      individual ray distribution on each composer shape.

      Global mode distributes rays over a circle with maximum
      shape radius, it may be used to ensure ray density is the
      same on each shape. Unobstructed ray tracing require global
      distribution too. */
    void use_global_distribution(bool use_global);

    /**
     @short Enable definition of shape as composition ot other shapes
     @header Goptical/Shape/Composer

     This class contains child shape transform and boolean
     operations for the @ref Composer shape class.
   */
    class Attributes
    {
        friend class TComposerShape;

        Attributes(const QSharedPointer<TShapeBase> &shape);

    public:

        /** Apply scaling affine transform using scale factors (xscale, yscale) */
        Attributes & scale(const Math::Vector2 &factor);
        /** Apply rotation affine transform. Angle is in degree. */
        Attributes & rotate(double dangle);
        /** Apply translation transform */
        Attributes & translate(const Math::Vector2 &offset);

        /** Peform boolean 'and' with the given shape */
        Attributes & include(const QSharedPointer<TShapeBase> &shape);
        /** Peform boolean 'and not' with the given shape */
        Attributes & exclude(const QSharedPointer<TShapeBase> &shape);

    private:
        bool inside(const Math::Vector2 &point) const;

        QSharedPointer<TShapeBase>         _shape;
        bool                               _exclude;
        std::list <Attributes>             _list;
        Math::Transform<2>                 _transform;
        Math::Transform<2>                 _inv_transform;
    };

    TComposerShape();

private:

    void update();
    void update() const;

    std::list <Attributes>    _list;
    bool                      _update;
    bool                      _global_dist;
    double                    _max_radius;
    double                    _min_radius;
    Math::VectorPair2         _bbox;
    unsigned int              _contour_cnt;
};

#endif // TCOMPOSER_H
