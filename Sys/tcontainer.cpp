#include "tcontainer.h"
#include <limits>
#include "Io/trgb.h"
#include "topticalsurface.h"
#include "tsurface.h"
#include "Io/trendererviewport.h"
#include "telement.h"
#include "Io/trenderer.h"
#include "Math/VectorPair"
#include "Math/Vector"

const TContainer::element_list_t & TContainer::get_element_list() const
{
    return _list;
}

template <class X> X* TContainer::find() const
{
    GOPTICAL_FOREACH(i, _list)
    {
        X *e;

        if ((e = dynamic_cast<X*>(i->data())))
            return e;

        TContainer *g;

        if ((g = dynamic_cast<TContainer*>(i->data())) &&
                (e = g->find<X>()))
            return e;
    }

    return 0;
}

template<> TOpticalSurface* TContainer::find<TOpticalSurface>() const
{
    GOPTICAL_FOREACH(i, _list)
    {
        TOpticalSurface *e;

        if ((e = dynamic_cast<TOpticalSurface*>(i->data())))
            return e;

        TContainer *g;

        if ((g = dynamic_cast<TContainer*>(i->data())) &&
                (e = g->find<TOpticalSurface>()))
            return e;
    }

    return 0;
}

template<> TSurface* TContainer::find<TSurface>() const
{
    GOPTICAL_FOREACH(i, _list)
    {
        TSurface *e;

        if ((e = dynamic_cast<TOpticalSurface*>(i->data())))
            return e;

        TContainer *g;

        if ((g = dynamic_cast<TContainer*>(i->data())) &&
                (e = g->find<TSurface>()))
            return e;
    }

    return 0;
}

template <class X>
void TContainer::get_elements(const delegate<void (const X &)> &d) const
{
    GOPTICAL_FOREACH(i, _list)
    {
        X     *e;

        if ((e = dynamic_cast<X*>(i->data())))
            d(*e);

        TContainer *g;

        if ((g = dynamic_cast<TContainer*>(i->data())))
            g->get_elements<X>(d);
    }
}

template <class X>
void TContainer::get_elements(const delegate<void (X &)> &d)
{
    GOPTICAL_FOREACH(i, _list)
    {
        X     *e;

        if ((e = dynamic_cast<X*>(i->data())))
            d(*e);

        TContainer *g;

        if ((g = dynamic_cast<TContainer*>(i->data())))
            g->get_elements<X>(d);
    }
}

template <class X>
void TContainer::enable_single(const X &e_)
{
    GOPTICAL_FOREACH(i, _list)
    {
        X     *e;

        if ((e = dynamic_cast<X*>(i->data())))
            e->set_enable_state(e == &e_);

        TContainer *g;

        if ((g = dynamic_cast<TContainer*>(i->data())))
            g->enable_single<X>(e_);
    }
}


TContainer::TContainer()
    : _list()
{
}

TContainer::~TContainer()
{
    // all TContainer elements become orphan
    GOPTICAL_FOREACH(i, _list)
            (*i)->_container = 0;
}

void TContainer::remove_all()
{
    while (!_list.empty())
        remove(*_list.front());
}

void TContainer::add(const QSharedPointer<TElement> &e)
{
    if (e->_container)
        e->_container->remove(*e);

    _list.push_back(e);

    e->_container = this;

    added(*e);
}

void TContainer::remove(TElement &e)
{
    removed(e);

    QSharedPointer<TElement> ptr(&e);

    assert(e._container == this);

    e._container = 0;

    _list.remove(ptr);
}

Math::VectorPair3 TContainer::get_bounding_box() const
{
    Math::Vector3 a(std::numeric_limits<double>::max());
    Math::Vector3 b(-std::numeric_limits<double>::max());

    GOPTICAL_FOREACH(i, get_element_list())
    {
        Math::VectorPair3 bi = (*i)->get_bounding_box();

        if (bi[0] == bi[1])
            continue;

        bi = (*i)->get_transform().transform_pair(bi);

        for (unsigned int j = 0; j < 3; j++)
        {
            if (bi[0][j] > bi[1][j])
                std::swap(bi[0][j], bi[1][j]);

            if (bi[0][j] < a[j])
                a[j] = bi[0][j];

            if (bi[1][j] > b[j])
                b[j] = bi[1][j];
        }
    }

    return Math::VectorPair3(a, b);
}

void TContainer::draw_2d_fit(TRendererViewport &r, bool keep_aspect) const
{
    Math::VectorPair3 b = get_bounding_box();

    r.set_window(Math::VectorPair2(b, 2, 1), keep_aspect);
    r.set_camera_direction(Math::vector3_100);
    r.set_camera_position(Math::vector3_0);

    r.set_feature_size(b[1].y() - b[0].y() / 20.);
}

void TContainer::draw_2d(TRenderer &r) const
{
    // optical axis
    Math::VectorPair3 b = get_bounding_box();
    r.draw_segment(Math::VectorPair2(b.z0(), 0., b.z1(), 0.),
                   rgb_gray);

    if (const TElement *e = dynamic_cast<const TElement*>(this))
        return r.draw_element_2d(*e, e);

    GOPTICAL_FOREACH(i, get_element_list())
            r.draw_element_2d(**i, 0);
}

void TContainer::draw_3d_fit(TRendererViewport &r, double z_offset) const
{
    Math::Transform<3> t;

    t.set_direction(-Math::vector3_100);
    t.translation_reset();

    Math::VectorPair3 b = get_bounding_box();
    std::swap(b[0].x(), b[0].z());
    std::swap(b[1].x(), b[1].z());

    Math::Vector3 center = (b[0] + b[1]) / 2.;
    double width = (center.x() - b[1].x()) / tan(Math::degree2rad(r.get_fov()) / 2.);
    double dist = width + (center.z() + b[0].z()) + z_offset;

    t.set_translation(t.transform(center) + Math::Vector3(dist, 0, 0));

    r.set_camera_transform(t.inverse());
    r.set_feature_size(fabs(width / 20.));
}

void TContainer::draw_3d(TRenderer &r) const
{
    if (const TElement *e = dynamic_cast<const TElement*>(this))
        return r.draw_element_3d(*e, e);

    GOPTICAL_FOREACH(i, get_element_list())
            r.draw_element_3d(**i, 0);
}

std::ostream & operator<<(std::ostream &o, const TContainer &c)
{
    GOPTICAL_FOREACH(i, c._list)
            o << "  " << **i << std::endl;

    return o;
}

