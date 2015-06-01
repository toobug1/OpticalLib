#include "tspot.h"
#include "Sys/tsystem.h"
#include "Io/trendereraxes.h"
#include "Data/tsampleset.h"
#include "Trace/tresult.h"

TSpot::TSpot(TSystem &system)
    : TPointImage(system),
      _processed_analysis(false)
{
    _axes.set_show_axes(false, TRendererAxes::XY);
    _axes.set_label("Saggital distance", TRendererAxes::X);
    _axes.set_label("Tangential distance", TRendererAxes::Y);
    _axes.set_unit("m", true, true, -3, TRendererAxes::XY);
}

void TSpot::process_trace()
{
    if (_processed_trace)
        return;

    trace();

    _centroid = _tracer.get_trace_result().get_intercepted_centroid(*_image);
}

void TSpot::process_analysis()
{
    if (_processed_analysis)
        return;

    process_trace();

    double    mean = 0;       // rms radius
    double    max = 0;        // max radius
    double    intensity = 0;  // total intensity

    GOPTICAL_FOREACH(i, *_intercepts)
    {
        double        dist = ((*i)->get_intercept_point() - _centroid).len();

        if (max < dist)
            max = dist;

        mean += Math::square(dist);
        intensity += (*i)->get_intensity();
    }

    _useful_radius = _max_radius = max;
    _rms_radius = sqrt(mean / _intercepts->size());
    _tot_intensity = intensity;

    _processed_analysis = true;
}

double TSpot::get_encircled_intensity(double radius)
{
    process_trace();

    double    intensity = 0;

    GOPTICAL_FOREACH(i, *_intercepts)
    {
        if (((*i)->get_intercept_point() - _centroid).len() <= radius)
            intensity += (*i)->get_intensity();
    }

    return intensity;
}

QSharedPointer<TPlot> TSpot::get_encircled_intensity_plot(int zones)
{
    TResult &result = _tracer.get_trace_result();
    const rays_queue_t &intercepts = result.get_intercepted(*_image);

    process_analysis();

    if (intercepts.empty())
        throw Error("no ray intercept found for encircled intensity plot");

    typedef std::map<double, QSharedPointer<TSampleSet> > data_sets_t;
    data_sets_t data_sets;

    // create plot data for each wavelen

    GOPTICAL_FOREACH(w, result.get_ray_wavelen_set())
    {
        QSharedPointer<TSampleSet> s /*= GOPTICAL_REFNEW(Data::SampleSet)*/;

        s->set_interpolation(Data::Linear);
        s->set_metrics(0.0, _useful_radius / (double)zones);
        s->resize(zones + 1);

        data_sets.insert(data_sets_t::value_type(*w, s));
    }

    // compute encircled intensity for each radius range

    GOPTICAL_FOREACH(i, *_intercepts)
    {
        double dist = ((*i)->get_intercept_point() - _centroid).len();

        if (dist > _useful_radius)
            continue;

        int n = (unsigned int)((zones - 1) * (dist / _useful_radius));

        assert(n >= 0 && n < zones);

        data_sets[(*i)->get_wavelen()]->get_y_value(n + 1) += (*i)->get_intensity();
    }

    // integrate

    QSharedPointer<TPlot> plot /*= GOPTICAL_REFNEW(Data::Plot)*/;

    GOPTICAL_FOREACH(d, data_sets)
    {
        for (int i = 1; i < zones; i++)
            d->second->get_y_value(i + 1) += d->second->get_y_value(i);

        TPlotData p(d->second);

        //      p.set_label("Encircled ray intensity"); FIXME set wavelen
        p.set_color(TSpectralLine::get_wavelen_color(d->first));
        p.set_style(Data::LinePlot);

        plot->add_plot_data(p);
    }

    plot->set_title("TSpot diagram encircled rays intensity");
    plot->get_axes().set_label("Distance from TSpot centroid", TRendererAxes::X);
    plot->get_axes().set_label("Encircled intensity", TRendererAxes::Y);
    plot->get_axes().set_unit("m", true, true, -3, TRendererAxes::X);
    plot->get_axes().set_unit("", false, false, 0, TRendererAxes::Y);

    return plot;
}

Math::Vector3 TSpot::get_center()
{
    process_analysis();

    return _tracer.get_trace_result().get_intercepted_center(*_image);
}

void TSpot::draw_spot(TRendererViewport &renderer)
{
    process_analysis();

    TResult &result = _tracer.get_trace_result();
    renderer.draw_intercepts(result, *_image);
}

void TSpot::draw_diagram(TRendererViewport &renderer, bool centroid_origin)
{
    process_analysis();

    TResult &result = _tracer.get_trace_result();
    Math::Vector2 center(result.get_intercepted_center(*_image), 0, 1);
    Math::Vector2 radius(_useful_radius, _useful_radius);

    renderer.set_window(Math::VectorPair2(center - radius, center + radius));

    _axes.set_position(_centroid);
    _axes.set_origin(centroid_origin ? _centroid : Math::vector3_0);
    _axes.set_tics_count(3, TRendererAxes::XY);
    renderer.draw_axes_2d(_axes);

    renderer.draw_intercepts(result, *_image);
}


void TSpot::invalidate()
{
    _processed_trace = false;
    _processed_analysis = false;
}

double TSpot::get_max_radius()
{
    process_analysis();

    return _max_radius;
}

double TSpot::get_rms_radius()
{
    process_analysis();

    return _rms_radius;
}

double TSpot::get_total_intensity()
{
    process_analysis();

    return _tot_intensity;
}

const Math::Vector3 & TSpot::get_centroid()
{
    process_analysis();

    return _centroid;
}

void TSpot::set_useful_radius(double radius)
{
    _useful_radius = radius;
}

TRendererAxes & TSpot::get_diagram_axes()
{
    return _axes;
}



