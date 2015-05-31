#include "tfocus.h"

TFocus::TFocus(TSystem &system)
    : TPointImage(system),
      _processed_focus(false)
{
}

void TFocus::process_focus()
{
    if (_processed_focus)
        return;

    trace();

    // find beam average vector

    double count = (double)_intercepts->size();
    Math::VectorPair3 beam_avg(Math::Vector3(0, 0, 0),
                               Math::Vector3(0, 0, 0));

    GOPTICAL_FOREACH(i, *_intercepts)
    {
        TTraceRay    &ray = **i;

        beam_avg.origin() += ray.get_position();
        beam_avg.direction() += ray.get_direction();
    }

    beam_avg.origin() /= count;
    beam_avg.direction().normalize();

    _best_focus.direction() = beam_avg.direction();

    // find the closest point on the average beam line for each ray

    std::vector<double>       list;

    list.reserve(_intercepts->size());

    GOPTICAL_FOREACH(i, *_intercepts)
    {
        TTraceRay    &ray = **i;

        const Math::Vector3   &u = beam_avg.direction();
        const Math::Vector3   &v = ray.get_direction();
        const Math::Vector3   w = beam_avg.origin() - ray.get_position();

        double        b = u * v;
        double        c = v * v;
        double        k = (u * u) * c - Math::square(b);

        if (k <= 1e-10)       // skip if beam line and ray almost parallel
            continue;

        double        s = (b * (v * w) - c * (u * w)) / k;

        if (s < 0)            // skip non convergent rays
            continue;

        list.push_back(s);
    }

    if (list.empty())
        throw Error("no convergent ray found for TFocus analysis");

    double    average = 0;

    GOPTICAL_FOREACH(i, list)
            average += /*square*/(*i);

    average /= (double)list.size();

    //      average = sqrt(average);

    _best_focus.origin() = beam_avg.origin() + beam_avg.direction() * average;

    _processed_focus = true;
}

void TFocus::invalidate()
{
    _processed_focus = false;
    _processed_trace = false;
}

const Math::VectorPair3 & TFocus::get_best_focus()
{
    process_focus();

    return _best_focus;
}



