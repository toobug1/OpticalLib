#include "tcomposershape.h"
#include "Trace/tdistribution.h"

#include <limits>

TComposerShape::Attributes & TComposerShape::Attributes::scale(const Math::Vector2 &factor)
{
    _transform.affine_scaling(factor);
    _inv_transform = _transform.inverse();

    return *this;
}

TComposerShape::Attributes & TComposerShape::Attributes::rotate(double angle)
{
    _transform.affine_rotation(0, angle);
    _inv_transform = _transform.inverse();

    return *this;
}

TComposerShape::Attributes & TComposerShape::Attributes::translate(const Math::Vector2 &offset)
{
    _transform.apply_translation(offset);
    _inv_transform = _transform.inverse();

    return *this;
}

void TComposerShape::use_global_distribution(bool use_global)
{
    _global_dist = use_global;
}

TComposerShape::TComposerShape()
    : _list(),
      _update(false),
      _global_dist(true),
      _max_radius(0.0),
      _min_radius(std::numeric_limits<double>::max()),
      _bbox(Math::vector2_pair_00),
      _contour_cnt(0)
{
}

TComposerShape::Attributes::Attributes(const QSharedPointer<TShapeBase> &shape)
    : _shape(shape),
      _list()
{
    _transform.reset();
    _inv_transform.reset();
}

TComposerShape::Attributes & TComposerShape::add_shape(const QSharedPointer<TShapeBase> &shape)
{
    _list.push_back(Attributes(shape));
    _update = true;
    return _list.back();
}

TComposerShape::Attributes & TComposerShape::Attributes::include(
        const QSharedPointer<TShapeBase> &shape)
{
    _list.push_back(Attributes(shape));
    _list.back()._exclude = false;
    return _list.back();
}

TComposerShape::Attributes & TComposerShape::Attributes::exclude(
        const QSharedPointer<TShapeBase> &shape)
{
    _list.push_back(Attributes(shape));
    _list.back()._exclude = true;
    return _list.back();
}

bool TComposerShape::Attributes::inside(const Math::Vector2 &point) const
{
    Math::Vector2 tp(_inv_transform.transform(point));

    bool res = _shape->inside(tp);

    GOPTICAL_FOREACH(s, _list)
            res &= s->inside(tp);

    return res ^ _exclude;
}

bool TComposerShape::inside(const Math::Vector2 &point) const
{
    GOPTICAL_FOREACH(s, _list)
            if (s->inside(point))
            return true;

    return false;
}

void TComposerShape::update()
{
    Math::Vector2 a(0);
    Math::Vector2 b(0);

    _contour_cnt = 0;

    GOPTICAL_FOREACH(s, _list)
    {
        // update max radius

        double m = s->_transform.transform(Math::vector2_0).len() + s->_shape->max_radius();

        if (m > _max_radius)
            _max_radius = m;
        if (m < _min_radius)
            _min_radius = m;

        // update bounding box

        Math::VectorPair2 bi = s->_transform.transform_pair(s->_shape->get_bounding_box());

        for (unsigned int j = 0; j < 2; j++)
        {
            if (bi[0][j] > bi[1][j])
                std::swap(bi[0][j], bi[1][j]);

            if (bi[0][j] < a[j])
                a[j] = bi[0][j];

            if (bi[1][j] > b[j])
                b[j] = bi[1][j];
        }

        // update contour count
        _contour_cnt += s->_shape->get_contour_count();
    }

    _bbox = Math::VectorPair2(a, b);
    _update = false;
}

void TComposerShape::update() const
{
    const_cast<TComposerShape*>(this)->update();
}

double TComposerShape::max_radius() const
{
    if (_update)
        update();

    return _max_radius;
}

double TComposerShape::min_radius() const
{
    if (_update)
        update();

    return _min_radius;
}

double TComposerShape::get_outter_radius(const Math::Vector2 &dir) const
{
    // FIXME
    return _max_radius;
}

double TComposerShape::get_hole_radius(const Math::Vector2 &dir) const
{
    // FIXME
    return 0;
}

Math::VectorPair2 TComposerShape::get_bounding_box() const
{
    if (_update)
        update();

    return _bbox;
}

void TComposerShape::get_pattern(const Math::Vector2::put_delegate_t &f,
                                 const TDistribution &d,
                                 bool unobstructed) const
{
    if (_global_dist)
    {
        TComposerShape::get_pattern(f, d, unobstructed);
    }
    else
    {
        GOPTICAL_FOREACH(s, _list)
        {
            DPP_DELEGATE3_OBJ(de, void, (const Math::Vector2 &v),
                              // _0
                              const Math::Vector2::put_delegate_t &, f,
                              // _1
                              const Math::Transform<2> &, s->_transform,
                              // _2
                              const std::list <Attributes> &, s->_list,
                              {
                                  Math::Vector2 p = _1.transform(v);

                                  GOPTICAL_FOREACH(s, _2)
                                  if (!s->_exclude ^ s->_shape->inside(s->_inv_transform.transform(p)))
                                  return;

                                  _0(p);
                               });

            s->_shape->get_pattern(de, d, unobstructed);
        }
    }
}

unsigned int TComposerShape::get_contour_count() const
{
    if (_update)
        update();

    return _contour_cnt;
}

void TComposerShape::get_contour(unsigned int contour, const Math::Vector2::put_delegate_t  &f, double resolution) const
{
    // FIXME add contour boolean operations

    GOPTICAL_FOREACH(s, _list)
    {
        unsigned int c = s->_shape->get_contour_count();

        if (contour < c)
        {
            DPP_DELEGATE2_OBJ(de, void, (const Math::Vector2 &v),
                              // _0
                              const Math::Vector2::put_delegate_t &, f,
                              // _1
                              const Math::Transform<2> &, s->_transform,
            {
                                  _0(_1.transform(v));
                              });

            return s->_shape->get_contour(contour, de, resolution);
        }

        contour -= c;
    }
}

void TComposerShape::get_triangles(const Math::Triangle<2>::put_delegate_t  &f, double resolution) const
{
    GOPTICAL_FOREACH(s, _list)
    {
        DPP_DELEGATE2_OBJ(de, void, (const Math::Triangle<2> &t),
                          // _0
                          const Math::Triangle<2>::put_delegate_t &, f,
                          // _1
                          const Math::Transform<2> &, s->_transform,
        {
                              Math::Triangle<2> p;

                              for (unsigned int i = 0; i < 3; i++)
                              p[i] = _1.transform(t[i]);

                              _0(p);
                          });

        s->_shape->get_triangles(de, resolution);
    }
}

