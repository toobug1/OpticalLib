#ifndef TREGULARPOLYGON_H
#define TREGULARPOLYGON_H

#include "tshapebase.h"

class TRegularPolygon : public TShapeBase
{
    /** Create a regular_polygon with given radius and edge count. */
    TRegularPolygon(double radius, unsigned int edge_cnt, double degree_angle = 0.);

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
    unsigned int get_contour_count() const;
    /** @override */
    void get_contour(unsigned int contour,
                     const Math::Vector2::put_delegate_t  &f,
                     double resolution) const;
    /** @override */
    void get_triangles(const Math::Triangle<2>::put_delegate_t &f, double resolution) const;

private:
    /** get edge segment associated with sector where specified point lies */
    inline Math::VectorPair2 get_edge(const Math::Vector2 &point) const;
    double get_radial_step(double resolution) const;

    double _radius;
    double _i_radius; // internal radius
    double _a_step;   // angle step
    double _edge_cnt;
    double _angle;
};

#endif // TREGULARPOLYGON_H
