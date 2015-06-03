#ifndef TRENDERER2D_H
#define TRENDERER2D_H

#include "trendererviewport.h"
#include "Math/Vector"
#include "Math/Transform"


class TRenderer2d : public TRendererViewport
{
protected:
    TRenderer2d();

    using TRenderer::draw_point;
    using TRenderer::draw_segment;
    using TRenderer::draw_text;

    /** project in 2d space and scale for ploting to 2d output */
    Math::Vector2 project_scale(const Math::Vector3 &v);

    /** project in 2d space */
    Math::Vector2 project(const Math::Vector3 &v);

    /** @override */
    void set_perspective();
    /** @override */
    void set_orthographic();
    /** @override */
    Math::Transform<3> get_camera_transform() const;
    /** @override */
    void set_camera_transform(const Math::Transform<3> & t);

    /** @override */
    void draw_point(const Math::Vector3 &p,
                    const TRgb &rgb,
                    enum PointStyle s);
    /** @override */
    void draw_segment(const Math::VectorPair3 &l, const TRgb &rgb);
    /** @override */
    void draw_text(const Math::Vector3 &pos,
                   const Math::Vector3 &dir,
                   const std::string &str,
                   TextAlignMask a,
                   int size,
                   const TRgb &rgb);

private:
    Math::Vector2 projection_ortho(const Math::Vector3 &v) const;
    Math::Vector2 projection_perspective(const Math::Vector3 &v) const;

    Math::Vector2 (TRenderer2d::*_projection)(const Math::Vector3 &v) const;

    Math::Transform<3> _cam_transform;
    double _eye_dist;
};

#endif // TRENDERER2D_H
