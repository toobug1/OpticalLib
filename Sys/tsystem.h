#ifndef TSYSTEM_H
#define TSYSTEM_H

#include <QSharedPointer>

#include "tcontainer.h"

#include "Material/tglass.h"
#include "Math/Transform"
#include "telement.h"

class TParams;
class TSurface;
class TTraceRay;

class TSystem : public TContainer
{
    friend class TElement;
public:
    /** Create a new empty system. */
    TSystem();
    virtual ~TSystem();

    /** Define an entrance pupil surface used to project source rays */
    void set_entrance_pupil(const QSharedPointer<TSurface> &entrance);
    /** Discard defined entrance pupil */
    void undef_entrance_pupil();
    /** Get defined entrance pupil surface or try to guess it if none defined */
    const TSurface &get_entrance_pupil() const;
    /** Test if an entrance pupil has been defined */
    bool has_entrance_pupil() const;

    /** Define an exit pupil surface */
    void set_exit_pupil(const QSharedPointer<TSurface> &exit);
    /** Get exit pupil */
    const TSurface &get_exit_pupil() const;
    /** Test if an exit pupil has been defined */
    bool has_exit_pupil() const;

    /** Get default tracer parameters */
    const TParams & get_tracer_params() const;
    /** Get default tracer parameters */
    TParams & get_tracer_params();

    /** Get transform between two elements local coordinates */
    const Math::Transform<3> & get_transform(const TElement &from,
                                                    const TElement &to) const;

    /** Get transform from element local to global coordinates */
    const Math::Transform<3> & get_global_transform(const TElement &from) const;

    /** Get transform from global to element local coordinates */
    const Math::Transform<3> & get_local_transform(const TElement &to) const;

    /** Get system version. version is updated each time system or
      associated elements properties are changed */
    unsigned int get_version() const;

    /** Get the number of registered elements in the system */
    unsigned int get_element_count() const;

    /** Get registered element. first element has index 1 */
    TElement & get_element(unsigned int index) const;

    /** Increase current system version */
    void update_version();

    /** Find surface which colides with the given ray and update intersection point */
    TSurface * colide_next(const TParams &params,
                          Math::VectorPair3 &intersect,
                          const TTraceRay &ray) const;

    /** set environment material */
    void set_environment(const QSharedPointer<TGlass> &env);

    /** get environment material */
    const TGlass & get_environment() const;

    /** @internal get environment material proxy */
    const TGlass & get_environment_proxy() const;

    /** @internal Dump 3d transforms cache */
    void transform_cache_dump(std::ostream &o) const;

private:

    /** called be container class when a new element is added */
    void added(TElement &e);
    /** called be container class when a new element is removed */
    void removed(TElement &e);

    /** get an new element identifier */
    unsigned int index_get(TElement &element);
    /** free the identifier associated with the given element */
    void index_put(const TElement &element);

    /** Get a reference to cache entry for transform between 2 elements (ids) */
    Math::Transform<3> * & transform_cache_entry(unsigned int from,
                                                        unsigned int to) const;

    /** Compute and get 3d transform between element local and global coordinates */
    const Math::Transform<3> & transform_l2g_cache_update(const TElement &e) const;
    /** Compute and get 3d transform between element local and global coordinates */
    const Math::Transform<3> & transform_g2l_cache_update(const TElement &e) const;
    /** Compute and get 3d transform between two elements local coordinates */
    const Math::Transform<3> & transform_cache_update(const TElement &from,
                                                      const TElement &to) const;

    /** Flush all cached transforms associated with a given element */
    void transform_cache_flush(const TElement &element);
    /** Flush all cached transforms */
    void transform_cache_flush();

    /** Resize transform cache size */
    void transform_cache_resize(unsigned int newsize);

    unsigned int              _version;

    QSharedPointer<TSurface>        _entrance;
    QSharedPointer<TSurface>        _exit;
    TGlass            _env_proxy;
    TParams*             _tracer_params;
    unsigned int              _e_count;
    std::vector<TElement *>    _index_map;
    // FIXME use transform pool instead of new/delete
    std::vector<Math::Transform<3> *> _transform_cache;
};

#endif // TSYSTEM_H
