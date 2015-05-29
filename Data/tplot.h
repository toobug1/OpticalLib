#ifndef TPLOT_H
#define TPLOT_H

#include "common.hh"
#include "tset.h"
#include "tplotdata.h"
#include "Io/trgb.h"
#include "Io/trendereraxes.h"
#include "Io/trendererviewport.h"

using namespace _Goptical::Data;

class TPlot
{
public:
    TPlot();

    /** Create and add plot data from specified data set. */
    TPlotData & add_plot_data(TSet &data,
                             const TRgb &color = rgb_red,
                             const std::string &label = "data",
                             PlotStyleMask style = InterpolatePlot | PointPlot);

    /** Add plot data */
    void add_plot_data(TPlotData &data);

    /** Discard all plot data set */
    void erase_plot_data();

    /** Get plot data set count */
    unsigned int get_plot_count() const;

    /** Get plot data set at given index */
    TPlotData & get_plot_data(unsigned int index);

    /** Get plot data set at given index */
    const TPlotData & get_plot_data(unsigned int index) const;

    /** Set plot main title */
    void set_title(const std::string & title);

    /** Get plot main title */
    const std::string & get_title() const;

    /** Set color for all plots */
    void set_color(const TRgb & color);

    /** Automatically choose different colors for each plot */
    void set_different_colors();

    /** Set plot style for all plot */
    void set_style(PlotStyleMask style);

    /** Swap x and y axis for 2d plots */
    void set_xy_swap(bool doswap);

    /** Get x and y axis swap state for 2d plots */
    bool get_xy_swap() const;

    /** Set axis position to dataset range */
    void fit_axes_range();

    /** Get plot axes object */
    TRendererAxes & get_axes();

    /** Get plot axes object */
    const TRendererAxes & get_axes() const;

    /** Get data sets dimensions, return 0 if inconsistent */
    unsigned int get_dimensions() const;

    /** Get range of x data in sets */
    Math::range_t get_x_data_range(unsigned int dimension = 0) const;

    /** Get range of y data in sets */
    Math::range_t get_y_data_range() const;

    /** draw */
    void draw(TRendererViewport &r);

private:

    double get_tic_step(const Math::range_t &r) const;

    std::string       _title;
    std::vector<TPlotData>     _plots;

    TRendererAxes  _axes;

    bool              _xy_swap;
};

#endif // TPLOT_H
