#ifndef TELLIPSE_H
#define TELLIPSE_H

#include "tshapebase.h"
#include "tshaperound.h"

class TEllipseBase : public TShapeBase
{
public:
    /** Set ellipse radius along x and y axis */
    void set_radius(double xr, double yr);
    /** Set ellipse radius along x axis */
    inline double get_x_radius(void) const;
    /** Set ellipse radius along y axis */
    inline double get_y_radius(void) const;

    /** @override */
    double max_radius() const;
    /** @override */
    double min_radius() const;
    /** @override */
    double get_outter_radius(const Math::Vector2 &dir) const;
    /** @override */
    Math::VectorPair2 get_bounding_box() const;
    /** @override */
    bool inside(const Math::Vector2 &point) const;

protected:

    double get_external_xradius() const;
    double get_internal_xradius() const;
    double get_xy_ratio() const;

    double _xr, _yr;
    double _xy_ratio;
    double _e2;
};

/**
   @short Ellipse shape
   @header Goptical/Shape/Ellipse
   @module {Core}
   @main
 */
class TEllipse : public TShapeRound<TEllipseBase, false>
{
public:
    /** Create a new ellipse with given radius */
    TEllipse(double x_radius, double y_radius);
};


#endif // TELLIPSE_H
