#ifndef TRENDEREROPENGL_H
#define TRENDEREROPENGL_H

#include <GL/gl.h>

#include "trendererviewport.h"


class TRendererOpengl : public TRendererViewport
{
public:

    TRendererOpengl(double dblNear, double dblFar,
                    double width = 800, double height = 600,
                    const TRgb &background = rgb_black);

    void set_z_range(double dblNear, double dblFar);

    static void glVertex(const Math::Vector2 &v);
    static void glVertex(const Math::Vector3 &v);
    static void glNormal(const Math::Vector3 &v);
    static void glColor(const TRgb &rgb);
    static void apply_transform(const Math::Transform<3> &t);
    static void get_transform(GLenum name, Math::Transform<3> &t);

private:
    /** @override */
    void clear();
    /** @override */
    void flush();

    /** @override */
    void set_2d_size(double width, double height);

    /** @override */
    void set_camera_transform(const Math::Transform<3> & t);
    /** @override */
    Math::Transform<3> get_camera_transform() const;
    /** @override */
    void set_orthographic();
    /** @override */
    void set_perspective();

    /** @override */
    void draw_point(const Math::Vector2 &p, const TRgb &rgb, enum PointStyle s);
    /** @override */
    void draw_segment(const Math::VectorPair2 &l, const TRgb &rgb);

    /** @override */
    void draw_point(const Math::Vector3 &p, const TRgb &rgb, enum PointStyle s);
    /** @override */
    void draw_segment(const Math::VectorPair3 &l, const TRgb &rgb);
    /** @override */
    void draw_polygon(const Math::Vector3 *array,
                      unsigned int count,
                      const TRgb &rgb,
                      bool filled,
                      bool closed);
    /** @override */
    void draw_triangle(const Math::Triangle<3> &t, const TRgb &rgb);
    /** @override */
    void draw_triangle(const Math::Triangle<3> &t,
                       const Math::Triangle<3> &gradient,
                       const TRgb &rgb);

    /** @override */
    void draw_text(const Math::Vector3 &pos, const Math::Vector3 &dir,
                   const std::string &str,
                   TextAlignMask a,
                   int size,
                   const TRgb &rgb);

    /** @override */
    void draw_text(const Math::Vector2 &pos,
                   const Math::Vector2 &dir,
                   const std::string &str,
                   TextAlignMask a,
                   int size,
                   const TRgb &rgb);

    double _near, _far;
};

#endif // TRENDEREROPENGL_H
