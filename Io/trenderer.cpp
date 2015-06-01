#include "trenderer.h"
#include "Sys/telement.h"
#include "Sys/tsurface.h"
#include "Sys/timage.h"
#include "Light/tspectralline.h"
#include "Trace/tresult.h"
#include "Error"

TRenderer::~TRenderer()
{
}

void TRenderer::set_ray_color_mode(RayColorMode m)
{
    _ray_color_mode = m;
}

void TRenderer::set_ray_intensity_mode(Io::IntensityMode m)
{
    _intensity_mode = m;
}

const TRgb & TRenderer::rgb_foreground() const
{
    return _styles_color[StyleForeground];
}

const TRgb & TRenderer::rgb_background() const
{
    return _styles_color[StyleBackground];
}

const TRgb & TRenderer::get_style_color(Style s) const
{
    assert(s < StyleLast);
    return _styles_color[s];
}

void TRenderer::set_style_color(Style s, const TRgb & rgb)
{
    assert(s < StyleLast);
    _styles_color[s] = rgb;
}

void TRenderer::draw_segment(const Math::Vector2 &a, const Math::Vector2 &b,
                             const TRgb & rgb)
{
    draw_segment(Math::VectorPair2(a, b), rgb);
}

void TRenderer::draw_segment(const Math::Vector3 &a, const Math::Vector3 &b,
                             const TRgb & rgb)
{
    draw_segment(Math::VectorPair3(a, b), rgb);
}

TRenderer::TRenderer()
    : _feature_size(20.),
      _ray_color_mode(RayColorWavelen),
      _intensity_mode(IntensityIgnore)
{
    _styles_color[StyleForeground] = TRgb(1.0, 1.0, 1.0);
    _styles_color[StyleBackground] = TRgb(0.0, 0.0, 0.0);
    _styles_color[StyleRay] = TRgb(1.0, 0.0, 0.0);
    _styles_color[StyleSurface] = TRgb(0.5, 0.5, 1.0);
    _styles_color[StyleGlass] = TRgb(0.8, 0.8, 1.0);
}

void TRenderer::group_begin(const std::string &name)
{
}

void TRenderer::group_end()
{
}

/**********************************************************************
 * Optical elements drawing
 */

void TRenderer::draw_element_2d(const TElement &e, const TElement *ref)
{
    group_begin("element");
    e.draw_2d_e(*this, ref);
    group_end();
}

void TRenderer::draw_element_3d(const TElement &e, const TElement *ref)
{
    group_begin("element");
    e.draw_3d_e(*this, ref);
    group_end();
}

/**********************************************************************
 * Light ray drawing
 */

template <unsigned D, bool draw_lost>
bool TRenderer::draw_traced_ray_recurs(const TTraceRay &ray, double lost_len,
                                       const TElement *ref, bool hit_image)
{
    const Math::Transform<3> &t1 = ray.get_creator()->get_transform_to(ref);
    Math::VectorPair3 p;
    TElement *i_element = 0;

    p[0] = t1.transform(ray.origin());

    if (ray.is_lost())
    {
        if (!draw_lost)
            return false;

        p[1] = t1.transform(ray.origin() + ray.direction() * lost_len);
    }
    else
    {
        i_element = &ray.get_intercept_element();
        const Math::Transform<3> &t2 = i_element->get_transform_to(ref);
        p[1] = t2.transform(ray.get_intercept_point());
    }

    bool done = false;

    for (TTraceRay *r = ray.get_first_child(); r; r = r->get_next_child())
        done |= draw_traced_ray_recurs<D, true>(*r, lost_len, ref, hit_image);

    if (!done && hit_image && !dynamic_cast<const TImage*>(i_element))
        return false;

    switch (D)
    {
    case 2:
        // skip non tangential rays in 2d mode
        if (fabs(p.x1()) > 1e-6)
            return false;

        draw_ray_line(Math::VectorPair2(p[0].project_zy(), p[1].project_zy()), ray);
        break;

    case 3:
        draw_ray_line(p, ray);
        break;
    }

    return true;
}

template <unsigned D>
void TRenderer::draw_trace_result(const TResult &result,
                                  const TElement *ref,
                                  bool hit_image)
{
    const TResult::sources_t &sl = result.get_source_list();
    double lost_len = result.get_params().get_lost_ray_length();

    if (sl.empty())
        throw Error("No source found in trace result");

    _max_intensity = result.get_max_ray_intensity();

    GOPTICAL_FOREACH(s, sl)
    {
        const rays_queue_t &rl = result.get_generated(*(TElement*)*s);

        GOPTICAL_FOREACH(r, rl)
        {
            group_begin("ray");
            draw_traced_ray_recurs<D, false>(**r, lost_len, ref, hit_image);
            group_end();
        }
    }
}

void TRenderer::draw_trace_result_2d(const TResult &result,
                                     bool hit_image,
                                     const TElement *ref)
{
    group_begin("rays");
    draw_trace_result<2>(result, ref, hit_image);
    group_end();
}

void TRenderer::draw_trace_result_3d(const TResult &result,
                                     bool hit_image,
                                     const TElement *ref)
{
    group_begin("rays");
    draw_trace_result<3>(result, ref, hit_image);
    group_end();
}

void TRenderer::draw_intercepts(const TResult &result, const TSurface &s)
{
    _max_intensity = result.get_max_ray_intensity();

    GOPTICAL_FOREACH(i, result.get_intercepted(s))
    {
        const TTraceRay &ray = **i;
        // dont need global transform here, draw ray intercept points in
        // surface local coordinates.
        draw_point(ray.get_intercept_point().project_xy(), ray_to_rgb(ray));
    }
}

const TRgb TRenderer::ray_to_rgb(const TRay & ray)
{
    switch (_ray_color_mode)
    {
    case RayColorWavelen:
        return TSpectralLine::get_wavelen_color(ray.get_wavelen());

    default:
    case RayColorFixed:
        return get_style_color(StyleRay);
    }
}

float TRenderer::ray_to_alpha(const TRay & ray) const
{
    switch (_intensity_mode)
    {
    case IntensityIgnore:
        return 0.0;

    case IntensityShade:
        return 1.0 - std::min(ray.get_intensity() / _max_intensity, 1.0);

    case IntensityLogShade: // FIXME add log
        return 1.0 - std::min(ray.get_intensity() / _max_intensity, 1.0);
    }

    return 0;
}

void TRenderer::draw_ray_line(const Math::VectorPair2 &l, const TTraceRay &ray)
{
    draw_segment(l, ray_to_rgb(ray));
}

void TRenderer::draw_ray_line(const Math::VectorPair3 &l, const TTraceRay &ray)
{
    draw_segment(l, ray_to_rgb(ray));
}

/**********************************************************************
 * Misc shapes 2d drawing
 */

void TRenderer::draw_polygon(const Math::Vector2 *array,
                             unsigned int count,
                             const TRgb &rgb, bool filled, bool closed)
{
    unsigned int i;

    if (count < 3)
        return;

    for (i = 0; i + 1 < count; i++)
        draw_segment(Math::VectorPair2(array[i], array[i + 1]), rgb);

    if (closed)
        draw_segment(Math::VectorPair2(array[i], array[0]), rgb);
}

void TRenderer::draw_circle(const Math::Vector2 &v,
                            double r,
                            const TRgb &rgb,
                            bool filled)
{
    unsigned int count = std::min(100, std::max(6, (int)(2. * M_PI * r / _feature_size)));
    DPP_VLARRAY(Math::Vector2, count, p);
    double astep = 2. * M_PI / count;
    double a = astep;
    p[0] = Math::Vector2(r, 0);

    for (unsigned int i = 0; i < count; i++, a += astep)
        p[i] = v + Math::Vector2(r * cos(a), r * sin(a));

    draw_polygon(&p[0], count, rgb, filled, true);
}

void TRenderer::draw_triangle(const Math::Triangle<3> &t, const TRgb &rgb)
{
    draw_polygon(&t[0], 3, rgb, false, true);
}

void TRenderer::draw_triangle(const Math::Triangle<3> &t,
                              const Math::Triangle<3> &gradient,
                              const TRgb &rgb)
{
    draw_triangle(t, rgb);
}

void TRenderer::draw_polygon(const Math::Vector3 *array, unsigned int count,
                             const TRgb &rgb, bool filled, bool closed)
{
    if (count < 3)
        return;

    unsigned int i;

    for (i = 0; i + 1 < count; i++)
        draw_segment(array[i], array[i + 1], rgb);

    if (closed)
        draw_segment(array[i], array[0], rgb);
}

void TRenderer::draw_box(const Math::VectorPair2 &c, const TRgb &rgb)
{
    draw_segment(Math::Vector2(c[0].x(), c[0].y()), Math::Vector2(c[1].x(), c[0].y()), rgb);
    draw_segment(Math::Vector2(c[1].x(), c[1].y()), Math::Vector2(c[1].x(), c[0].y()), rgb);
    draw_segment(Math::Vector2(c[1].x(), c[1].y()), Math::Vector2(c[0].x(), c[1].y()), rgb);
    draw_segment(Math::Vector2(c[0].x(), c[0].y()), Math::Vector2(c[0].x(), c[1].y()), rgb);
}

void TRenderer::draw_triangle(const Math::Triangle<2> &t, bool filled, const TRgb &rgb)
{
    draw_polygon(&t[0], 3, rgb, filled, true);
}


