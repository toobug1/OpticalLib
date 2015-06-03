#include "trenderer2d.h"

TRenderer2d::TRenderer2d()
{
    _projection = &TRenderer2d::projection_ortho;
    _cam_transform.reset();
}

void TRenderer2d::set_orthographic()
{
    _projection = &TRenderer2d::projection_ortho;
}

void TRenderer2d::set_perspective()
{
    double out_ratio = (_2d_output_res.y() / _rows) / (_2d_output_res.x() / _cols);

    if (out_ratio < 1.)
        _window2d = Math::VectorPair2(-1. / out_ratio, -1., 1. / out_ratio, 1.);
    else
        _window2d = Math::VectorPair2(-1, -out_ratio, 1., out_ratio);

    _window2d_fit = _window2d;
    update_2d_window();
    set_page(_pageid);

    _projection = &TRenderer2d::projection_perspective;
    _eye_dist = 1. / tan(Math::degree2rad(_fov) / 2.);
}

Math::Vector2 TRenderer2d::projection_ortho(const Math::Vector3 &v) const
{
    Math::Vector2 p(_cam_transform.transform(v).project_xy());

    return p;
}

Math::Vector2 TRenderer2d::projection_perspective(const Math::Vector3 &v) const
{
    Math::Vector3 t(_cam_transform.transform(v));

    Math::Vector2 p(t.x() * _eye_dist / -t.z(),
                    t.y() * _eye_dist / -t.z());

    return p;
}

Math::Transform<3> TRenderer2d::get_camera_transform() const
{
    return _cam_transform;
}

void TRenderer2d::set_camera_transform(const Math::Transform<3> & t)
{
    _cam_transform = t;
}

void TRenderer2d::draw_point(const Math::Vector3 &p,
                             const TRgb &rgb,
                             enum PointStyle s)
{
    draw_point(project(p), rgb, s);
}

void TRenderer2d::draw_segment(const Math::VectorPair3 &l, const TRgb &rgb)
{
    draw_segment(Math::VectorPair2(project(l[0]), project(l[1])), rgb);
}

void TRenderer2d::draw_text(const Math::Vector3 &pos,
                            const Math::Vector3 &dir,
                            const std::string &str,
                            TextAlignMask a,
                            int size,
                            const TRgb &rgb)
{
    draw_text(project(pos), project(dir), str, a, size, rgb);
}

Math::Vector2 TRenderer2d::project_scale(const Math::Vector3 &v)
{
    Math::Vector2 v2d(project(v));

    return Math::Vector2(x_trans_pos(v2d.x()), y_trans_pos(v2d.y()));
}

Math::Vector2 TRenderer2d::project(const Math::Vector3 &v)
{
    return (this->*_projection)(v);
}

