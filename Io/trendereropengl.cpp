#include "trendereropengl.h"

#include <GL/glu.h>

TRendererOpengl::TRendererOpengl(double near, double far,
                                 double width, double height,
                                 const TRgb &bg)
    : _near(near),
      _far(far)
{
    set_2d_size(width, height);

    _styles_color[StyleBackground] = bg;
    _styles_color[StyleForeground] = ~bg;
}

void TRendererOpengl::set_2d_size(double width, double height)
{
    TRendererViewport::set_2d_size(width, height);
    glViewport(0, 0, width, height);
}

void TRendererOpengl::set_camera_transform(const Math::Transform<3> & t)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    apply_transform(t);
}

Math::Transform<3> TRendererOpengl::get_camera_transform() const
{
    Math::Transform<3> t;
    get_transform(GL_MODELVIEW_MATRIX, t);
    return t;
}

void TRendererOpengl::set_orthographic()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(_window2d[0].x(), _window2d[1].x(),
            _window2d[0].y(), _window2d[1].y(), _near, _far);
}

void TRendererOpengl::set_perspective()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(_fov, _2d_output_res.x() / _2d_output_res.y(), _near, _far);
}

void TRendererOpengl::set_z_range(double dblNear, double dblFar)
{
    _near = dblNear;
    _far = dblFar;
}

void TRendererOpengl::draw_point(const Math::Vector2 &p,
                                const TRgb &TRgb, enum PointStyle s)
{
    glBegin(GL_POINTS);
    glColor(TRgb);
    glVertex(p);
    glEnd();
}

void TRendererOpengl::draw_segment(const Math::VectorPair2 &l, const TRgb &TRgb)
{
    glBegin(GL_LINES);
    glColor(TRgb);
    glVertex(l[0]);
    glVertex(l[1]);
    glEnd();
}

void TRendererOpengl::draw_point(const Math::Vector3 &p,
                                const TRgb &TRgb, enum PointStyle s)
{
    glBegin(GL_POINTS);
    glColor(TRgb);
    glVertex(p);
    glEnd();
}

void TRendererOpengl::draw_segment(const Math::VectorPair3 &l, const TRgb &TRgb)
{
    glBegin(GL_LINES);
    glColor(TRgb);
    glVertex(l[0]);
    glVertex(l[1]);
    glEnd();
}

void TRendererOpengl::draw_text(const Math::Vector2 &c, const Math::Vector2 &dir,
                               const std::string &str, TextAlignMask a, int size, const TRgb &TRgb)
{
    // FIXME
}

void TRendererOpengl::draw_text(const Math::Vector3 &c, const Math::Vector3 &dir,
                               const std::string &str, TextAlignMask a, int size, const TRgb &TRgb)
{
    // FIXME
}

void TRendererOpengl::draw_polygon(const Math::Vector3 *array,
                                  unsigned int count, const TRgb &TRgb,
                                  bool filled, bool closed)
{
    if (count < 3)
        return;

    if (filled)
    {
        glBegin(GL_POLYGON);
        glColor(TRgb);
        for (unsigned int i = 0; i < count; i++)
            glVertex(array[i]);
        glEnd();
    }
    else
    {
        glBegin(GL_LINE_STRIP);
        for (unsigned int i = 0; i < count; i++)
            glVertex(array[i]);
        if (closed)
            glVertex(array[0]);
        glEnd();
    }
}

void TRendererOpengl::draw_triangle(const Math::Triangle<3> &t, const TRgb &TRgb)
{
    glBegin(GL_TRIANGLES);
    glColor(TRgb);
    glNormal(t.normal());

    for (unsigned int i = 0; i < 3; i++)
        glVertex(t[i]);

    glEnd();

#ifdef CONFIG_OPENGL_DRAWNORMAL
    glBegin(GL_LINES);
    for (unsigned int i = 0; i < 3; i++)
    {
        glVertex(t[i]);
        glVertex(t[i] + t.normal() * 20.0);
    }
    glEnd();
#endif
}

void TRendererOpengl::draw_triangle(const Math::Triangle<3> &t, const Math::Triangle<3> &gradient, const TRgb &TRgb)
{
    glBegin(GL_TRIANGLES);
    glColor(TRgb);

    for (unsigned int i = 0; i < 3; i++)
    {
        glNormal(gradient[i]);
        glVertex(t[i]);
    }

    glEnd();

#ifdef CONFIG_OPENGL_DRAWNORMAL
    glBegin(GL_LINES);
    for (unsigned int i = 0; i < 3; i++)
    {
        glVertex(t[i]);
        glVertex(t[i] + gradient[i] * 20.0);
    }
    glEnd();
#endif
}

void TRendererOpengl::clear()
{
    const TRgb & TRgb = _styles_color[StyleBackground];
    glClearColor(TRgb.r, TRgb.g, TRgb.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TRendererOpengl::flush()
{
    glFlush();
}

void TRendererOpengl::apply_transform(const Math::Transform<3> &t)
{
    GLdouble m[16];

    m[ 0] = t.get_linear().value(0, 0);
    m[ 1] = t.get_linear().value(1, 0);
    m[ 2] = t.get_linear().value(2, 0);
    m[ 3] = 0.0;

    m[ 4] = t.get_linear().value(0, 1);
    m[ 5] = t.get_linear().value(1, 1);
    m[ 6] = t.get_linear().value(2, 1);
    m[ 7] = 0.0;

    m[ 8] = t.get_linear().value(0, 2);
    m[ 9] = t.get_linear().value(1, 2);
    m[10] = t.get_linear().value(2, 2);
    m[11] = 0.0;

    m[12] = t.get_translation().x();
    m[13] = t.get_translation().y();
    m[14] = t.get_translation().z();
    m[15] = 1.0;

    glMultMatrixd(m);
}

void TRendererOpengl::get_transform(GLenum name, Math::Transform<3> &t)
{
    GLdouble m[16];

    glGetDoublev(name, m);

    t.get_linear().value(0, 0) = m[ 0];
    t.get_linear().value(1, 0) = m[ 1];
    t.get_linear().value(2, 0) = m[ 2];

    t.get_linear().value(0, 1) = m[ 4];
    t.get_linear().value(1, 1) = m[ 5];
    t.get_linear().value(2, 1) = m[ 6];

    t.get_linear().value(0, 2) = m[ 8];
    t.get_linear().value(1, 2) = m[ 9];
    t.get_linear().value(2, 2) = m[10];

    t.get_translation().x()    = m[12];
    t.get_translation().y()    = m[13];
    t.get_translation().z()    = m[14];
}


void TRendererOpengl::glVertex(const Math::Vector2 &v)
{
    glVertex2d(v.x(), v.y());
}

void TRendererOpengl::glVertex(const Math::Vector3 &v)
{
    glVertex3d(v.x(), v.y(), v.z());
}

void TRendererOpengl::glNormal(const Math::Vector3 &v)
{
    glNormal3d(v.x(), v.y(), v.z());
}

void TRendererOpengl::glColor(const TRgb &TRgb)
{
    glColor4f(TRgb.r, TRgb.g, TRgb.b, TRgb.a);
}

