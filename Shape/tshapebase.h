#ifndef TSHAPEBASE_H
#define TSHAPEBASE_H

#include "common.hh"
#include "Math/Vector"
#include "Math/Triangle"
#include "Trace/tdistribution.h"

using namespace _Goptical;
using namespace dpp;

class TShapeBase
{
public:
    TShapeBase(){}

    virtual ~TShapeBase() {}

    /** Check if the (x,y) 2d point is inside 2d shape area */
    virtual bool inside(const Math::Vector2 &point) const = 0;

    /** Get points distributed on shape area with given pattern */
    virtual void get_pattern(const Math::Vector2::put_delegate_t &f,
                             const TDistribution &d,
                             bool unobstructed = false) const;

    /** Get distance between origin and farthest shape edge */
    virtual double max_radius() const = 0;

    /** Get distance between origin and nearest shape outter edge */
    virtual double min_radius() const = 0;

    /** Get distance between origin and farthest shape edge in specified direction */
    virtual double get_outter_radius(const Math::Vector2 &dir) const = 0;

    /** Get distance between origin and nearest shape outter edge in specified direction */
    virtual double get_hole_radius(const Math::Vector2 &dir) const;

    /** Get shape bounding box */
    virtual Math::VectorPair2 get_bounding_box() const = 0;

    /** Get number of contours polygones. This function returns
      value is greater than 1 if shape has hole(s). @see get_contour */
    virtual unsigned int get_contour_count() const = 0;

    /** Get contour polygone points for specified contour id. First
      contour is always outter edge. @see get_contour_count */
    virtual void get_contour(unsigned int contour,
                             const Math::Vector2::put_delegate_t &f,
                             double resolution) const = 0;

    /** Get shape teselation triangles */
    virtual void get_triangles(const Math::Triangle<2>::put_delegate_t &f,
                               double resolution) const = 0;
};

#endif // TSHAPEBASE_H
