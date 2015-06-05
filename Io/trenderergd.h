#ifndef TRENDERERGD_H
#define TRENDERERGD_H


extern "C" {
#include <gd.h>
#include <gdfontl.h>
}

#include "trenderer2d.h"

class TRendererGd : public TRenderer2d
{
public:
    /** Create a new gd renderer using specified gd resources */
    TRendererGd(gdImagePtr im, const TRgb &background = rgb_white,
               gdFontPtr font = 0, const char *TFfontlist = 0);

    /** Create a new gd renderer with given resolution, png output
      will be written to file on renderer destruction. */
    TRendererGd(const char *filename, unsigned int width = 800,
               unsigned int height = 600, const TRgb &background = rgb_white,
               const char *TFfontlist = 0);

    /** Create a new gd renderer with given resolution */
    TRendererGd(unsigned int width = 800, unsigned int height = 600,
               const TRgb &background = rgb_white, const char *TFfontlist = 0);

    ~TRendererGd();

    void set_brush_thickness(unsigned int t);

    /** Write image to file in png format */
    void write(const std::string &filename);

    /** Clear current image buffer */
    void clear();

private:

    /** @override */
    void set_2d_size(double width, double height);

    /** @override */
    void draw_ray_line(const Math::VectorPair3 &l, const TTraceRay &ray);

    /** @override */
    void draw_point(const Math::Vector2 &p, const TRgb &rgb, enum PointStyle s);
    /** @override */
    void draw_segment(const Math::VectorPair2 &l, const TRgb &rgb);
    /** @override */
    void draw_circle(const Math::Vector2 &c, double r, const TRgb &rgb, bool filled);
    /** @override */
    void draw_text(const Math::Vector2 &pos, const Math::Vector2 &dir,
                   const std::string &str, TextAlignMask a, int size, const TRgb &rgb);
    /** @override */
    void draw_polygon(const Math::Vector2 *array, unsigned int count,
                      const TRgb &rgb, bool filled, bool closed);

    double y_trans_pos(double y) const;
    Math::Vector2 trans_pos(const Math::Vector2 &v);

    void init(const TRgb &background);

    int rgb_to_gdcolor(const TRgb &rgb, float alpha) const;
    int rgb_to_gdcolor(const TRgb &rgb) const;
    int style_to_gdcolor(Style style) const;
    int ray_to_gdcolor(const TRay & ray);

    bool _user_im;
    const char *_filename;
    unsigned int _thickness;

    gdImagePtr _im;
    gdFontPtr _font;
    const char *_fontlist;
};

#endif // TRENDERERGD_H
