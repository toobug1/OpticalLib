#include "tplot.h"

#include <limits>

TPlot::TPlot()
    : _title(),
      _plots(),
      _axes(),
      _xy_swap(false)
{
}

void TPlot::set_title(const std::string & title)
{
    _title = title;
}

TPlotData & TPlot::add_plot_data(TSet &data, const TRgb &color,
                               const std::string &label, PlotStyleMask style)
{
    _plots.push_back(TPlotData(QSharedPointer<TSet>(&data)));

    _plots.back().set_color(color);
    _plots.back().set_label(label);
    _plots.back().set_style(style);

    return _plots.back();
}

void TPlot::add_plot_data(TPlotData &data)
{
    _plots.push_back(data);
}

void TPlot::erase_plot_data()
{
    _plots.clear();
}

Math::range_t TPlot::get_x_data_range(unsigned int dimension) const
{
    Math::range_t r(std::numeric_limits<double>::max(),
                    std::numeric_limits<double>::min());

    GOPTICAL_FOREACH(i, _plots)
    {
        Math::range_t ri = i->get_set().get_x_range(dimension);

        if (ri.first < r.first)
            r.first = ri.first;

        if (ri.second > r.second)
            r.second = ri.second;
    }

    return r;
}

Math::range_t TPlot::get_y_data_range() const
{
    Math::range_t r(std::numeric_limits<double>::max(),
                    std::numeric_limits<double>::min());

    GOPTICAL_FOREACH(i, _plots)
    {
        Math::range_t ri = i->get_set().get_y_range();

        if (ri.first < r.first)
            r.first = ri.first;

        if (ri.second > r.second)
            r.second = ri.second;
    }

    return r;
}

unsigned int TPlot::get_dimensions() const
{
    unsigned int dimension = 0;

    GOPTICAL_FOREACH(i, _plots)
    {
        unsigned int d = i->get_set().get_dimensions();

        if (dimension == 0)
            dimension = d;
        else
            if (dimension != d)
                return 0;
    }

    return dimension;
}

void TPlot::set_color(const TRgb & color)
{
    GOPTICAL_FOREACH(i, _plots)
            i->set_color(color);
}

void TPlot::set_different_colors()
{
    TRgb color;
    unsigned int n = 1;

    GOPTICAL_FOREACH(i, _plots)
    {
        color.r = (double)((n >> 0) & 0x01);
        color.g = (double)((n >> 1) & 0x01);
        color.b = (double)((n >> 2) & 0x01);

        i->set_color(color);
        n++;
    }
}

void TPlot::set_style(PlotStyleMask style)
{
    GOPTICAL_FOREACH(i, _plots)
            i->set_style(style);
}

void TPlot::fit_axes_range()
{
    switch (get_dimensions())
    {
    case 1:
        _axes.set_range(get_x_data_range(0), TRendererAxes::X);
        _axes.set_range(get_y_data_range(), TRendererAxes::Y);
        break;
    case 2:
        _axes.set_range(get_x_data_range(0), TRendererAxes::X);
        _axes.set_range(get_x_data_range(1), TRendererAxes::Y);
        _axes.set_range(get_y_data_range(), TRendererAxes::Z);
        break;
    default:
        throw Error("inconsistent dimensions of data sets in TPlot");
    }
}

void TPlot::draw(TRendererViewport &r)
{
    r.draw_plot(*this);
}


const std::string & TPlot::get_title() const
{
    return _title;
}

unsigned int TPlot::get_plot_count() const
{
    return _plots.size();
}

TRendererAxes & TPlot::get_axes()
{
    return _axes;
}

const TRendererAxes & TPlot::get_axes() const
{
    return _axes;
}

TPlotData & TPlot::get_plot_data(unsigned int index)
{
    assert(index < _plots.size());
    return _plots[index];
}

const TPlotData & TPlot::get_plot_data(unsigned int index) const
{
    assert(index < _plots.size());
    return _plots[index];
}

void TPlot::set_xy_swap(bool doswap)
{
    _xy_swap = doswap;
}

bool TPlot::get_xy_swap() const
{
    return _xy_swap;
}


