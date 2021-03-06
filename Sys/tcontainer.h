#ifndef TCONTAINER_H
#define TCONTAINER_H

#include <iostream>
#include <list>

#include <QSharedPointer>

#include "common.hh"
#include "delegate"

class TRendererViewport;
class TElement;
class TRenderer;

using namespace _Goptical;
using namespace dpp;

class TContainer
{
    friend std::ostream & operator<<(std::ostream &o, const TContainer &s);
    friend class TElement;

public:
    typedef std::list<QSharedPointer<TElement> > element_list_t;

    TContainer();
    virtual ~TContainer();

    /** Add an element */
    void add(const QSharedPointer<TElement> &e);

    /** Remove an element */
    void remove(TElement &e);

    /** Find first element of type X in container and subcontainers */
    template <class X>  X* find() const;

    /** Invoke a delegate for each element of type X in
        container and subcontainers */
    template <class X>
    void get_elements(const delegate<void (const X &)> &d) const;

    /** Invoke a modifier delegate for each element of type X in
        container and subcontainers */
    template <class X>
    void get_elements(const delegate<void (X &)> &d);

    /** Disable all elements of specified type which are not specified element */
    template <class X>
    void enable_single(const X &e);

    /** Test if element is contained in container and subcontainers */
    template <class X>
    bool contains(const X *x) const;

    /** Return a reference to container children list */
    const element_list_t & get_element_list() const;

    /** Get system or element group bounding box */
    Math::VectorPair3 get_bounding_box() const;

    /** Setup the TRenderer 2d viewport to best fit for this
        system or element group. @pb This function calls
        @ref TRenderer::set_window and @ref TRenderer::set_feature_size. */
    void draw_2d_fit(TRendererViewport &r, bool keep_aspect = true) const;

    /** Draw system 2d layout using specified TRenderer. @see draw_2d_fit */
    void draw_2d(TRenderer &r) const;

    /** Move the TRenderer 3d camera to best fit for this
        system or element group. @pb This function calls
        @ref Io::RendererViewport::set_camera_transform
        and @ref TRenderer::set_feature_size. @pb
    */
    void draw_3d_fit(TRendererViewport &r, double z_offset = 0) const;

    /** Draw system in 3d using specified TRenderer. @see draw_3d_fit */
    void draw_3d(TRenderer &r) const;

protected:

    /** remove all elements in container */
    void remove_all();

    /** called when elements are inserted or removed */
    virtual void added(TElement &e) = 0;
    /** called when elements are inserted or removed */
    virtual void removed(TElement &e) = 0;

private:

    element_list_t            _list;
};

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

#endif // TCONTAINER_H
