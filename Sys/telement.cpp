#include "telement.h"
#include "tsystem.h"
#include "tcontainer.h"
#include "tgroup.h"
#include "Math/Vector"
#include "Math/VectorPair"


#include <typeinfo>

void TElement::print(std::ostream &o) const
{
    o << " [" << id() << "]" << typeid(*this).name() << " at " << get_position();
}

std::ostream & operator<<(std::ostream &o, const TElement &e)
{
    e.print(o);
    return o;
}

unsigned int TElement::id() const
{
    return _system_id;
}

Math::Vector3 TElement::get_position() const
{
    return (_system->get_global_transform(*this)).transform(Math::vector3_0);
}

const Math::Transform<3> & TElement::get_global_transform() const
{
    assert(_system);
    return _system->get_global_transform(*this);
}

const Math::Transform<3> & TElement::get_transform_to(const TElement &e) const
{
    assert(_system);
    return _system->get_transform(*this, e);
}

const Math::Transform<3> & TElement::get_transform_to(const TElement *e) const
{
    assert(_system);
    return e ? _system->get_transform(*this, *e)
             : _system->get_global_transform(*this);
}


void TElement::set_local_position(const Math::Vector3 &v)
{
    _transform.set_translation(v);
    system_moved();
}

void TElement::rotate(double x, double y, double z)
{
    _transform.linear_rotation(Math::Vector3(x, y, z));
    system_moved();
}

const Math::Vector3 & TElement::get_local_position() const
{
    return _transform.get_translation();
}

const Math::Transform<3> & TElement::get_transform() const
{
    return _transform;
}

void TElement::set_transform(const Math::Transform<3> &t)
{
    _transform = t;
    system_moved();
}

TSystem * TElement::get_system() const
{
    return _system;
}

unsigned int TElement::get_version() const
{
    return _version;
}

bool TElement::is_enabled() const
{
    return _enabled;
}

void TElement::set_enable_state(bool enabled)
{
    if (_enabled != enabled)
        update_version();
    _enabled = enabled;
}




TElement::TElement(const Math::VectorPair3 &plane)
    : _system(0),
      _container(0),
      _enabled(true),
      _version(0),
      _system_id(0),
      _transform()
{
    set_local_plane(plane);
}

TElement::~TElement()
{
    if (_container)
        _container->remove(*this);
}

void TElement::set_local_direction(const Math::Vector3 &v)
{
    _transform.set_direction(v);
    system_moved();
}

Math::Vector3 TElement::get_local_direction() const
{
    return _transform.transform_linear(Math::vector3_001);
}

Math::Vector3 TElement::get_position(const TElement &e) const
{
    return _system->get_transform(*this, e).transform(Math::vector3_0);
}

Math::Vector3 TElement::get_direction(const TElement &e) const
{
    return _system->get_transform(*this, e).transform_linear(Math::vector3_001);
}

//Math::Vector3 TElement::get_position() const
//{
//    return _system->get_global_transform(*this).transform(Math::vector3_0);
//}

void TElement::set_position(const Math::Vector3 &v)
{
    if (TElement *parent = get_parent())
        set_local_position(_system->get_local_transform(*parent).transform(v));
    else
        set_local_position(v);
}

Math::Vector3 TElement::get_direction() const
{
    return _system->get_global_transform(*this).transform_linear(Math::vector3_001);
}

void TElement::set_direction(const Math::Vector3 &v)
{
    if (TElement *parent = get_parent())
        set_local_direction(_system->get_local_transform(*parent).transform_linear(v));
    else
        set_local_direction(v);
}

Math::VectorPair3 TElement::get_plane() const
{
    const Math::Transform<3> &t = _system->get_global_transform(*this);

    return Math::VectorPair3(t.transform(Math::vector3_0),
                             t.transform_linear(Math::vector3_001));
}

Math::VectorPair3 TElement::get_plane(const TElement &e) const
{
    const Math::Transform<3> &t = _system->get_transform(*this, e);

    return Math::VectorPair3(t.transform(Math::vector3_0),
                             t.transform_linear(Math::vector3_001));
}

void TElement::set_plane(const Math::VectorPair3 &p)
{
    if (TElement *parent = get_parent())
    {
        const Math::Transform<3> &t = _system->get_local_transform(*parent);
        _transform.set_translation(t.transform(p[0]));
        set_local_direction(t.transform_linear(p[1]));
    }
    else
    {
        set_local_plane(p);
    }
    // system_moved(); called in set_local_direction
}

Math::VectorPair3 TElement::get_local_plane() const
{
    return Math::VectorPair3(_transform.get_translation(),
                             _transform.transform_linear(Math::vector3_001));
}

void TElement::set_local_plane(const Math::VectorPair3 &p)
{
    _transform.set_translation(p[0]);
    set_local_direction(p[1]);
    // system_moved(); called in set_local_direction
}

Math::VectorPair3 TElement::get_bounding_box() const
{
    return Math::VectorPair3(Math::vector3_0, Math::vector3_0);
}

//const Math::Transform<3> & TElement::get_transform_to(const TElement &e) const
//{
//    assert(_system);
//    return _system->get_transform(*this, e);
//}

const Math::Transform<3> & TElement::get_transform_from(const TElement &e) const
{
    assert(_system);
    return _system->get_transform(e, *this);
}

//const Math::Transform<3> & TElement::get_transform_to(const TElement *e) const
//{
//    assert(_system);
//    return e ? _system->get_transform(*this, *e)
//             : _system->get_global_transform(*this);
//}

const Math::Transform<3> & TElement::get_transform_from(const TElement *e) const
{
    assert(_system);
    return e ? _system->get_transform(*e, *this)
             : _system->get_local_transform(*this);
}

//const Math::Transform<3> & TElement::get_global_transform() const
//{
//    assert(_system);
//    return _system->get_global_transform(*this);
//}

const Math::Transform<3> & TElement::get_local_transform() const
{
    assert(_system);
    return _system->get_local_transform(*this);
}

TGroup * TElement::get_parent() const
{
    return dynamic_cast<TGroup*>(_container);
}

void TElement::process_rays_simple(TResult &result,
                                  rays_queue_t *input) const
{
    throw Error("this element is not designed to process incoming light rays in simple ray trace mode");
}

void TElement::process_rays_intensity(TResult &result,
                                     rays_queue_t *input) const
{
    throw Error("this element is not designed to process incoming light rays in intensity ray trace mode");
}

void TElement::process_rays_polarized(TResult &result,
                                     rays_queue_t *input) const
{
    throw Error("this element is not designed to process incoming light rays in polarized ray trace mode");
}

void TElement::system_register(TSystem &s)
{
    assert(!_system);
    _system = &s;
    _system_id = s.index_get(*this);
}

void TElement::system_unregister()
{
    assert(_system);
    _system->index_put(*this);
    _system = 0;
    _system_id = 0;
}

void TElement::system_moved()
{
    if (_system)
        _system->transform_cache_flush(*this);
    update_version();
}

void TElement::update_version()
{
    TElement *e;

    for (e = this; e; e = dynamic_cast<TElement *>(e->_container))
        e->_version++;

    if (_system)
        _system->update_version();
}

void TElement::draw_2d_e(TRenderer &r, const TElement *ref) const
{
}

void TElement::draw_3d_e(TRenderer &r, const TElement *ref) const
{
}

