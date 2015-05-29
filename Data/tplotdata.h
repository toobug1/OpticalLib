#ifndef TPLOTDATA_H
#define TPLOTDATA_H

#include <QSharedPointer>

#include "tplot.h"
#include "tset.h"
#include "Io/trgb.h"

class TPlotData
{
    friend class TPlot;

public:
    /** Create a new data plot descriptor which describe the
        specified dataset. */
    TPlotData(const QSharedPointer<TSet> &s);

    /** Get the described data set */
    const TSet & get_set() const;

    /** Set data set plotting label */
    void set_label(const std::string & title);
    /** Get data set plotting label */
    const std::string & get_label() const;

    /** Set data set plotting color */
    void set_color(const TRgb & color);
    /** Set data set plotting color */
    const TRgb & get_color() const;

    /** Enable a plotting style */
    void enable_style(PlotStyleMask style);
    /** Disable a plotting style */
    void disable_style(PlotStyleMask style);
    /** Set the plotting style mask */
    void set_style(PlotStyleMask style);
    /** Get the plotting style mask */
    PlotStyleMask get_style() const;

private:
    QSharedPointer<TSet>    _set;
    TRgb                    _color;
    PlotStyleMask           _style;
    std::string             _label;

};

#endif // TPLOTDATA_H
