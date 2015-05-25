#ifndef TDISK_H
#define TDISK_H

#include "tshapebase.h"
#include "tshaperound.h"

class TDiskBase : public TShapeBase
{
public:
    /** Set disk radius */
    void set_radius(double r);
    /** Get disk radius */
    double get_radius(void) const;

protected:

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

    double get_external_xradius() const;
    double get_internal_xradius() const;
    double get_xy_ratio() const;

    double _radius;
};

/**
   @short Disk shape
   @header Goptical/Shape/Disk
   @module {Core}
   @main

   This is the most common lens shape.
 */

class TDisk : public TShapeRound<TDiskBase, false>
{
public:
    /** Create a new disk with given radius */
    TDisk(double radius);
};


#endif // TDISK_H
