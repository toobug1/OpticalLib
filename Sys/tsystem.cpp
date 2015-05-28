#include "tsystem.h"
#include "tsurface.h"
#include "Trace/ttraceray.h"

#include "Error"
#include "Sys/tsource.h"
#include "topticalsurface.h"
#include "Trace/tparams.h"
#include "Trace/ttraceray.h"
#include "Material/tair.h"

#include <limits>


// TODO

Math::Transform<3> * & TSystem::transform_cache_entry(unsigned int from, unsigned int to) const
{
    return const_cast<Math::Transform<3> * &>(_transform_cache[from * _e_count + to]);
}

const Math::Transform<3> & TSystem::get_global_transform(const TElement &from) const
{
    Math::Transform<3> * &e = transform_cache_entry(from.id(), 0);

    // TODO
    if (!e)
        transform_l2g_cache_update(from);

    return *e;
}

const Math::Transform<3> & TSystem::transform_l2g_cache_update(const TElement &element) const
{
    Math::Transform<3> * & e = transform_cache_entry(element.id(), 0);

    // TODO
    if (!e)
    {
        Math::Transform<3> t(element._transform);
        const TElement *i1 = &element;

        //          while (const TElement *i2 =
        //                                 (dynamic_cast<TGroup *>(i1->_container)))
        //            {
        //              t.compose(i2->_transform);

        //              i1 = i2;
        //            }

        assert(!e);
        e = new Math::Transform<3>(t);
    }

    return *e;
}

const Math::Transform<3> & TSystem::transform_g2l_cache_update(const TElement &element) const
{
    Math::Transform<3> * & e = transform_cache_entry(0, element.id());

    if (!e)
        e = new Math::Transform<3>(transform_l2g_cache_update(element).inverse());

    return *e;
}

const Math::Transform<3> & TSystem::get_transform(const TElement &from, const TElement &to) const
{
    Math::Transform<3> * &e = transform_cache_entry(from.id(), to.id());
    // TODO
    //    if (!e)
    //        transform_cache_update(from, to);

    return *e;
}

void TSystem::set_entrance_pupil(const QSharedPointer<TSurface> &entrance)
{
    _entrance = entrance;
    update_version();
}

void TSystem::undef_entrance_pupil()
{
    _entrance.clear();
    update_version();
}

void TSystem::set_exit_pupil(const QSharedPointer<TSurface> &exit)
{
    _exit = exit;
    update_version();
}

const TSurface & TSystem::get_exit_pupil() const
{
    if (_exit.isNull())
        throw Error("system has no exit pupil defined");

    return *_exit;
}

bool TSystem::has_exit_pupil() const
{
    return !_exit.isNull();
}

bool TSystem::has_entrance_pupil() const
{
    return !_entrance.isNull();
}

const TParams & TSystem::get_tracer_params() const
{
    return *_tracer_params;
}

TParams & TSystem::get_tracer_params()
{
    update_version();
    return *_tracer_params;
}


const Math::Transform<3> &
TSystem::get_local_transform(const TElement &to) const
{
    Math::Transform<3> * &e = transform_cache_entry(0, to.id());

    if (!e)
        transform_g2l_cache_update(to);

    return *e;
}

void TSystem::update_version()
{
    _version++;
}

unsigned int TSystem::get_version() const
{
    return _version;
}

unsigned int TSystem::get_element_count() const
{
    return _e_count - 1;
}

TElement & TSystem::get_element(unsigned int index) const
{
    assert(index > 0 && index <= _e_count);
    return *_index_map[index];
}

const TGlass & TSystem::get_environment() const
{
    TGlass mat(_env_proxy);
    return mat;
}

const TGlass & TSystem::get_environment_proxy() const
{
    return _env_proxy;
}

TSystem::TSystem()
    : _version(0),
      _env_proxy(air),
      _tracer_params(),
      _e_count(0),
      _index_map(),
      _transform_cache()
{
    transform_cache_resize(1);
    // index 0 is reserved for global coordinates transformations
    _index_map[0] = (TElement*)1;
}

TSystem::~TSystem()
{
    transform_cache_flush();
    remove_all();
}

void TSystem::added(TElement &e)
{
    e.system_register(*this);

    update_version();
}

void TSystem::removed(TElement &e)
{
    e.system_unregister();

    if (_entrance.data() == &e)
        _entrance.clear();

    if (_exit.data() == &e)
        _exit.clear();

    update_version();
}

void TSystem::set_environment(const TGlass &env)
{
    update_version();
    _env_proxy = env;
}


const Math::Transform<3> &
TSystem::transform_cache_update(const TElement &from,
                                const TElement &to) const
{
    assert(&from != &to);

    Math::Transform<3> * & e = transform_cache_entry(from.id(), to.id());

    if (!e)
    {
        const Math::Transform<3> &t1 = transform_l2g_cache_update(from);
        const Math::Transform<3> &t2 = transform_l2g_cache_update(to);

        if (!e)
        {
            e = new Math::Transform<3>(t1);
            e->compose(t2.inverse());
        }
    }

    return *e;
}

void TSystem::transform_cache_flush(const TElement &element)
{
    for (unsigned int x = 0; x < _e_count; x++)
    {
        if (Math::Transform<3> * & t =
                transform_cache_entry(element.id(), x))
        {
            delete t;
            t = 0;
        }

        if (Math::Transform<3> * & t =
                transform_cache_entry(x, element.id()))
        {
            delete t;
            t = 0;
        }
    }
}

void TSystem::transform_cache_flush()
{
    for (unsigned int x = 0; x < _e_count; x++)
        for (unsigned int y = 0; y < _e_count; y++)
            if (Math::Transform<3> * & t = transform_cache_entry(x, y))
            {
                delete t;
                t = 0;
            }
}

void TSystem::transform_cache_resize(unsigned int newsize)
{
    unsigned int oldsize = _e_count;

    if (oldsize < newsize)
    {
        _index_map.resize(newsize, 0);
        _transform_cache.resize(newsize * newsize, 0);

        for (int y = oldsize - 1; y > 0; y--)
        {
            // move data
            for (int x = oldsize - 1; x >= 0; x--)
            {
                _transform_cache[y * newsize + x] =
                        _transform_cache[y * oldsize + x];

                _transform_cache[y * oldsize + x] = 0;
            }
        }

        _e_count = newsize;
    }
    else
    {
        // FIXME handle cache downsize
    }

}

unsigned int TSystem::index_get(TElement &element)
{
    std::vector<TElement*>::iterator i;
    unsigned int index;

    i = std::find(_index_map.begin(), _index_map.end(), (TElement*)0);

    if (i == _index_map.end())
    {
        index = _e_count;
        transform_cache_resize(index + 1);
    }
    else
    {
        index = _index_map.begin() - i;
    }

    _index_map[index] = &element;

    return index;
}

void TSystem::index_put(const TElement &element)
{
    transform_cache_flush(element);
    _index_map[element.id()] = 0;
}

void TSystem::transform_cache_dump(std::ostream &o) const
{
    o << "System transform cache size is " << _e_count << "x" << _e_count << std::endl;

    for (unsigned int from = 0; from < _e_count; from++)
        for (unsigned int to = 0; to < _e_count; to++)
            if (const Math::Transform<3> *t = _transform_cache[from * _e_count + to])
                o << "from " << from << " to " << to << ":" << std::endl << *t << std::endl;
}

const TSurface & TSystem::get_entrance_pupil() const
{
    const TSurface *res = 0;

    if (!_entrance.isNull())
        return *_entrance;

    res = find<TOpticalSurface>();

    if (!res)
        res = find<TSurface>();

    if (!res)
        throw Error("No entrance pupil surface defined/guessed");

    return *res;
}

/** FIXME write an optimized version of this function which uses
    some kind of data structure (bsp tree?) */
TSurface *TSystem::colide_next(const TParams &params,
                             Math::VectorPair3 &intersect,
                             const TTraceRay &ray) const
{
    const TElement *origin = ray.get_creator();

    // test all elements and keep closest intersection

    TSurface *s, *e = 0;
    Math::VectorPair3 inter;
    double    min_dist = std::numeric_limits<double>::max();

    for (unsigned int i = 1; i <= get_element_count(); i++)
    {
        TElement *j = &get_element(i);

        if (j == origin || !j->is_enabled())
            continue;

        if ((s = dynamic_cast<TSurface*>(j)))
        {
            const Math::Transform<3> &t = origin->get_transform_to(*s);
            Math::VectorPair3 local(t.transform_line(ray));

            if (s->intersect(params, inter, local))
            {
                double        dist = (inter.origin() - local.origin()).len();

                if (min_dist > dist)
                {
                    min_dist = dist;
                    intersect = inter;
                    e = s;
                }
            }
        }
    }

    return e;
}

