#ifndef TRING_H
#define TRING_H

#include "tshapebase.h"
#include "tshaperound.h"

class TRingBase : public TShapeBase
{
public:
    /** Set ring external radius and hole radius */
    void set_radius(double radius, double hole_radius);

    /** Get ring external radius */
    double get_radius() const;
    /** Get hole radius */
    double get_hole_radius() const;

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

    double _radius, _hole_radius;
};

/**
   @short Ring shape
   @header Goptical/Shape/Ring
   @module {Core}
   @main

   This is a disk shape with a hole in center.
 */

class TRing : public TShapeRound<TRingBase, true>
{
public:
    /** Create a new ring with given external radius and hole radius. */
    TRing(double radius, double hole_radius);
};

#endif // TRING_H
