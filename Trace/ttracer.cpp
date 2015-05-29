#include "ttracer.h"
#include "Math/VectorPair"
#include "Error"
#include "tdistribution.h"
#include "tsequence.h"
#include "Sys/tsurface.h"
#include "Sys/telement.h"
#include "Sys/tsource.h"
#include "ttraceray.h"

void TTracer::set_trace_result(TResult &res)
{
    _result_ptr = &res;
}

TResult & TTracer::get_trace_result() const
{
    return *_result_ptr;
}

TResult & TTracer::set_default_trace_result()
{
    return *(_result_ptr = &_result);
}

const TSystem & TTracer::get_system() const
{
    return *_system;
}

const TParams & TTracer::get_params() const
{
    return _params;
}

TParams & TTracer::get_params()
{
    return _params;
}

TTracer::TTracer(QSharedPointer<TSystem> system)
    : _system(system),
      _params(system->get_tracer_params()),
      _result(),
      _result_ptr(&_result)
{
}

TTracer::~TTracer()
{
}

template <Trace::IntensityMode m> void TTracer::trace_seq_template()
{
    TResult &result = *_result_ptr;

    result.init(*_system);

    // stack of rays to propagate
    rays_queue_t tmp[2];

    unsigned int swaped = 0;
    rays_queue_t *generated;
    rays_queue_t *source_rays = &tmp[1];
    const std::vector<QSharedPointer<const TElement> > &seq =
            _params._sequence->_list;
    const TElement *entrance = 0;

    // find entry element (first non source)
    for (unsigned int i = 0; i < seq.size(); i++)
    {
        if (!dynamic_cast<const TSource *>(seq[i].data()))
        {
            entrance = seq[i].data();
            break;
        }
    }

    for (unsigned int i = 0; i < seq.size(); i++)
    {
        const TElement *element = seq[i].data();

        if (_system != element->get_system())
            throw Error("Sequence contains element which is not part of the System");

        if (!element->is_enabled())
            continue;

        TResult::element_result_s &er =
                result.get_element_result(*element);

        generated = er._generated ? er._generated : &tmp[swaped];
        result._generated_queue = generated;
        generated->clear();

        if (const TSource *source =
                dynamic_cast<const TSource *>(element))
        {
            result._sources.push_back(source);
            TSource::targets_t elist;
            if (entrance)
                elist.push_back(entrance);
            source->generate_rays<m>(result, elist);
        }
        else
        {
            element->process_rays<m>(result, source_rays);
            // swap ray buffers
        }

        GOPTICAL_DEBUG(" " << generated->size() << " rays generated by " << *element);
        source_rays = generated;
        swaped ^= 1;
    }

    result._generated_queue = 0;
}

template <Trace::IntensityMode m> void TTracer::trace_template()
{
    TResult &result = *_result_ptr;

    result.init(*_system);

    if (_params._propagation_mode != RayPropagation)
        throw Error("Diffractive propagation not supported in non sequential mode");

    // stack of rays to propagate

    rays_queue_t source_rays;

    TSource::targets_t entry;
    entry.push_back(&_system->get_entrance_pupil());

    // FIXME avoid container use here
    std::vector<const TSource *> slist;
    delegate_push<typeof(slist), const TSource &> d(slist);
    _system->get_elements<TSource>(d);

    GOPTICAL_FOREACH(s, slist)
    {
        const TSource &source = **s;

        if (_system != source.get_system())
            throw Error("can not trace with Source which is not part of the System");

        if (!source.is_enabled())
            continue;

        result._sources.push_back(&source);

        // get rays from source
        source_rays.clear();
        result._generated_queue = &source_rays;
        source.generate_rays<m>(result, entry);

        // copy to source generated rays
        {
            TResult::element_result_s &source_er =
                    result.get_element_result(source);

            if (source_er._generated)
                *source_er._generated = source_rays;
        }

        GOPTICAL_DEBUG("NSeq Ray Trace: " << source_rays.size() << " Rays");

        // trace each ray generated by source through the system

        rays_queue_t gqueue;
        result._generated_queue = &gqueue;

        GOPTICAL_FOREACH(r, source_rays)
        {
            TTraceRay *ray = *r;
            unsigned int bounce = _params._max_bounce;

            // trace relfected/refracted ray further
            while (1)
            {
                // check bounce limit
                if (!bounce--)
                    result._bounce_limit_count++;
                else
                {
                    Math::VectorPair3 intersect; // intersection point and normal (intersect surface local)

                    // find ray / surface interction
                    if (TSurface *s =
                            _system->colide_next(_params, intersect, *ray))
                    {
                        result.add_intercepted(*s, *ray);

                        // transform incident ray to surface local
                        const Math::Transform<3> &t = ray->get_creator()->get_transform_to(*s);
                        Math::VectorPair3 local(t.transform_line(*ray));

                        s->trace_ray<m>(result, *ray, local, intersect);
                    }
                }

                // pick next ray to trace further through the system
                if (gqueue.empty())
                    break;

                ray = gqueue.front();
                gqueue.pop_front();

                result.add_generated(*ray->get_creator(), *ray);
            }
        }
    }

    result._generated_queue = 0;
}

void TTracer::trace()
{
    TResult &result = *_result_ptr;

    // clear previous results
    result.prepare();

    result._params = &_params;

    switch (_params._intensity_mode)
    {
    case SimpleTrace:
        if (!_params._sequential_mode)
            trace_template<SimpleTrace>();
        else
            trace_seq_template<SimpleTrace>();
        break;

    case IntensityTrace:
        if (!_params._sequential_mode)
            trace_template<IntensityTrace>();
        else
            trace_seq_template<IntensityTrace>();
        break;

    case PolarizedTrace:
        if (!_params._sequential_mode)
            trace_template<PolarizedTrace>();
        else
            trace_seq_template<PolarizedTrace>();
        break;
    }
}

