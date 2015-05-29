#ifndef TRENDERER_H
#define TRENDERER_H

#include "common.hh"
#include "trgb.h"
#include "Math/Vector"
#include "Math/VectorPair"
#include "Math/Triangle"
#include "Light/tray.h"

class TResult;
class TElement;
class TSurface;
class TTraceRay;

using namespace _Goptical::Io;

class TRenderer
{
public:
    TRenderer();

    virtual ~TRenderer();

    GOPTICAL_ACCESSORS(double, feature_size,
                       "size of lines and triangles used to render curved shapes.")

    /** Set color mode for light ray drawing. Default is @ref
        RayColorWavelen. */
    void set_ray_color_mode(RayColorMode m);

    /** Set light ray intensity display mode */
    void set_ray_intensity_mode(Io::IntensityMode m);

    /** Get foreground rgb color */
    const TRgb & rgb_foreground() const;

    /** Get foreground rgb color */
    const TRgb & rgb_background() const;

    /** Get rgb color from style specifier */
    const TRgb & get_style_color(Style s) const;

    /** Set rgb color for given style specifier. @pb Some drivers do not
        support changing background color after initialization.  */
    void set_style_color(Style s, const TRgb &rgb);

    ////////////// Internal stuff

    /** @internal Draw tangential rays found in trace result using 2d primitives. */
    virtual void draw_trace_result_2d(const TResult &result,
                                      bool hit_image,
                                      const TElement *ref);
    /** @internal Draw rays found in trace result using 3d primitives. */
    virtual void draw_trace_result_3d(const TResult &result,
                                      bool hit_image, const
                                      TElement *ref);
    /** @internal Draw all optical elements in container using 2d primitives. */
    virtual void draw_element_2d(const TElement &e,
                                 const TElement *ref);
    /** @internal Draw all optical elements in container using 3d primitives. */
    virtual void draw_element_3d(const TElement &e,
                                 const TElement *ref);
    /** @internal Draw point corresponding to ray intercepts on a surface */
    virtual void draw_intercepts(const TResult &result,
                                 const TSurface &s);

    /** @internal ray line drawing in global coordinate, called by @ref draw_traced_ray_recurs */
    virtual void draw_ray_line(const Math::VectorPair3 &l,
                               const TTraceRay &ray);
    /** @internal ray line drawing in global coordinate, called by @ref draw_traced_ray_recurs */
    virtual void draw_ray_line(const Math::VectorPair2 &l,
                               const TTraceRay &ray);

    /** @internal Draw a point in 2d */
    virtual void draw_point(const Math::Vector2 &p,
                            const TRgb &rgb = rgb_gray,
                            enum PointStyle s = PointStyleDot) = 0;
    /** @internal Draw a line segment in 2d */
    virtual void draw_segment(const Math::VectorPair2 &s,
                              const TRgb &rgb = rgb_gray) = 0;
    /** @internal Draw a line segment in 2d */
    void draw_segment(const Math::Vector2 &a,
                             const Math::Vector2 &b,
                             const TRgb &rgb = rgb_gray);
    /** @internal Draw a circle in 2d */
    virtual void draw_circle(const Math::Vector2 &v,
                             double r,
                             const TRgb &rgb = rgb_gray,
                             bool filled = false);
    /** @internal Draw text in 2d */
    virtual void draw_text(const Math::Vector2 &pos,
                           const Math::Vector2 &dir,
                           const std::string &str,
                           TextAlignMask a,
                           int size,
                           const TRgb &rgb = rgb_gray) = 0;
    /** @internal Draw polygon in 2d */
    virtual void draw_polygon(const Math::Vector2 *array,
                              unsigned int count,
                              const TRgb &rgb = rgb_gray,
                              bool filled = false, bool closed = true);
    /** @internal Draw a box in 2d */
    void draw_box(const Math::VectorPair2 &corners,
                  const TRgb &rgb = rgb_gray);
    /** @internal Draw a triangle in 2d */
    void draw_triangle(const Math::Triangle<2> &t,
                       bool filled = false,
                       const TRgb &rgb = rgb_gray);

    /** @internal Draw a point in 3d */
    virtual void draw_point(const Math::Vector3 &p,
                            const TRgb &rgb = rgb_gray,
                            enum PointStyle s = PointStyleDot) = 0;
    /** @internal Draw a line segment in 3d */
    virtual void draw_segment(const Math::VectorPair3 &l,
                              const TRgb &rgb = rgb_gray) = 0;
    /** @internal Draw a line segment in 3d */
    void draw_segment(const Math::Vector3 &a,
                             const Math::Vector3 &b,
                             const TRgb &rgb = rgb_gray);
    /** @internal Draw polygon in 3d */
    virtual void draw_polygon(const Math::Vector3 *array,
                              unsigned int count,
                              const TRgb &rgb = rgb_gray,
                              bool filled = false,
                              bool closed = true);
    /** @internal Draw text in 3d */
    virtual void draw_text(const Math::Vector3 &pos,
                           const Math::Vector3 &dir,
                           const std::string &str,
                           TextAlignMask a, int size,
                           const TRgb &rgb = rgb_gray) = 0;

    /** @internal Draw filled triangle in 3d */
    virtual void draw_triangle(const Math::Triangle<3> &t,
                               const TRgb &rgb);
    /** @internal Draw filled triangle in 3d */
    virtual void draw_triangle(const Math::Triangle<3> &t,
                               const Math::Triangle<3> &gradient,
                               const TRgb &rgb);

    /** @internal Get alpha channel from ray intensity and intensity mode */
    float ray_to_alpha(const TRay & ray) const;
    /** @internal Get rgb color from ray wavalen and ray color mode */
    const TRgb ray_to_rgb(const TRay & ray);

    /** @internal Begin group */
    virtual void group_begin(const std::string &name = "");
    /** @internal End group */
    virtual void group_end();

protected:

    void init_styles();

    double _feature_size;

    TRgb              _styles_color[StyleLast];
    RayColorMode      _ray_color_mode;
    Io::IntensityMode     _intensity_mode;
    float             _max_intensity; // max ray intensity updated on

private:

    template <unsigned D>
    void draw_trace_result(const TResult &result,
                           const TElement *ref,
                           bool hit_image);
    template <unsigned D, bool draw_lost>
    bool draw_traced_ray_recurs(const TTraceRay &ray,
                                double lost_len,
                                const TElement *ref,
                                bool hit_image);
};

#endif // TRENDERER_H
