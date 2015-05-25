#ifndef TELLIPTICALRING_H
#define TELLIPTICALRING_H

#include "tshapebase.h"
#include "tshaperound.h"


class TEllipticalRingBase : public TShapeBase
{
public:
    /** Set external ring radius along the x and y axes and hole
      radius along the x axis. */
    void set_radius(double x_radius, double y_radius, double x_hole_radius);

    /** Get external ring radius along the x axis */
    double get_x_radius(void) const;
    /** Get external ring radius along the y axis */
    double get_y_radius(void) const;

    /** Get hole radius along the x axis */
    double get_x_hole_radius(void) const;
    /** Get hole radius along the y axis */
    double get_y_hole_radius(void) const;

    /** @override */
    double max_radius() const;
    /** @override */
    double min_radius() const;
    /** @override */
    double get_outter_radius(const Math::Vector2 &dir) const;
    /** @override */
    double get_hole_radius(const Math::Vector2 &dir) const;
    /** @override */
    Math::VectorPair2 get_bounding_box() const;
    /** @override */
    bool inside(const Math::Vector2 &point) const;

protected:

    double get_external_xradius() const;
    double get_internal_xradius() const;
    double get_xy_ratio() const;

    double _xr, _yr, _xhr;
    double _xy_ratio;
    double _e2;
};

/**
   @short Elliptical ring shape
   @header Goptical/Shape/EllipticalRing
   @module {Core}
   @main

   This is an ellipse shape with a hole in center.
 */
class TEllipticalRing : public TShapeRound<TEllipticalRingBase, true>
{
public:
    /** Create a new elliptical ring with given external ring radius
      along the x and y axes and hole radius along the x axis. */
    TEllipticalRing(double x_radius, double y_radius, double x_hole_radius);
};


#endif // TELLIPTICALRING_H
