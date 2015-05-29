#include "tplotdata.h"


TPlotData::TPlotData(const QSharedPointer<TSet> &s)
    : _set(s),
      _color(rgb_red),
      _style(InterpolatePlot | PointPlot),
      _label("")
{
}

const TSet & TPlotData::get_set() const
{
    return *_set;
}

void TPlotData::set_label(const std::string & label)
{
    _label = label;
}

const std::string & TPlotData::get_label() const
{
    return _label;
}

void TPlotData::set_color(const TRgb & color)
{
    _color = color;
}

const TRgb & TPlotData::get_color() const
{
    return _color;
}

void TPlotData::enable_style(PlotStyleMask style)
{
    _style = _style | style;
}

void TPlotData::disable_style(PlotStyleMask style)
{
    _style = _style & ~style;
}

void TPlotData::set_style(PlotStyleMask style)
{
    _style = style;
}

PlotStyleMask TPlotData::get_style() const
{
    return _style;
}



