#ifndef TPOLYGON_H
#define TPOLYGON_H

#include "tshapebase.h"
#include "Math/VectorPair"

class TPolygon : public TShapeBase
{
public:
    /** Create a polygon with given radius and edge count */
    TPolygon();

    unsigned int add_vertex(const Math::Vector2 &v);
    void insert_vertex(const Math::Vector2 &v, unsigned int id);
    void delete_vertex(unsigned int id);

    inline unsigned int get_vertices_count() const;
    inline const Math::Vector2 & get_vertex(unsigned int id);

private:

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
    /** @override */
    inline unsigned int get_contour_count() const;
    /** @override */
    void get_contour(unsigned int contour,
                     const Math::Vector2::put_delegate_t  &f,
                     double resolution) const;
    /** @override */
    void get_triangles(const Math::Triangle<2>::put_delegate_t &f, double resolution) const;

    /** update _min_radius and bounding box */
    void update();

    typedef std::vector<Math::Vector2 > vertices_t;

    bool _updated;
    vertices_t _vertices;
    Math::VectorPair2 _bbox;
    double _max_radius;
    double _min_radius;
};

#endif // TPOLYGON_H
