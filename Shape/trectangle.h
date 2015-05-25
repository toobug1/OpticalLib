#ifndef TRECTANGLE_H
#define TRECTANGLE_H

#include "tshapebase.h"
#include "Math/Vector"

class TRectangle : public TShapeBase
{
public:
    /** Create a rectangle with given width and height */
    inline TRectangle(double width, double height);
    /** Create a square with given side length */
    inline TRectangle(double sqsize);

    /** @override */
    inline double max_radius() const;
    /** @override */
    inline double min_radius() const;
    /** @override */
    double get_outter_radius(const Math::Vector2 &dir) const;
    /** @override */
    Math::VectorPair2 get_bounding_box() const;
    /** @override */
    bool inside(const Math::Vector2 &point) const;
    /** @override */
    void get_pattern(const Math::Vector2::put_delegate_t  &v,
                     const TDistribution &d,
                     bool unobstructed) const;
    /** @override */
    unsigned int get_contour_count() const;
    /** @override */
    void get_contour(unsigned int contour,
                     const Math::Vector2::put_delegate_t  &f,
                     double resolution) const;
    /** @override */
    void get_triangles(const Math::Triangle<2>::put_delegate_t &f, double resolution) const;

private:
    Math::Vector2 get_step(double resolution) const;

    Math::Vector2 _halfsize;
};

#endif // TRECTANGLE_H
