#include "tlens.h"
#include "Curve/tflat.h"
#include "Curve/tsphere.h"
#include "Shape/tdisk.h"
#include "tsurface.h"
#include "Io/trenderer.h"
#include "Material/tdielectricbase.h"
#include "tstop.h"

TLens::TLens(const Math::VectorPair3 &p, double offset,
             const QSharedPointer<TGlass> &env)
    : TGroup(p),
      _last_pos(offset),
      _surfaces(_surfaces_storage),
      _next_mat(env)
{
}

TLens::TLens(const Math::VectorPair3 &p,
             const QSharedPointer<TCurveBase> &curve0,
             const QSharedPointer<TShapeBase> &shape0,
             const QSharedPointer<TCurveBase> &curve1,
             const QSharedPointer<TShapeBase> &shape1,
             double thickness0,
             const QSharedPointer<TGlass> &glass0,
             const QSharedPointer<TGlass> &env)
    : TGroup(p),
      _last_pos(0),
      _surfaces(_surfaces_storage),
      _next_mat(env)
{
    _surfaces.reserve(2);
    add_surface(curve0, shape0, thickness0, glass0);
    add_surface(curve1, shape1, 0, env);
}

TLens::TLens(const Math::VectorPair3 &p,
             double roc0, double ap_radius0,
             double roc1, double ap_radius1,
             double thickness0,
             const QSharedPointer<TGlass> &glass0,
             const QSharedPointer<TGlass> &env)
    : TGroup(p),
      _last_pos(0),
      _surfaces(_surfaces_storage),
      _next_mat(env)
{
    _surfaces.reserve(2);
    add_surface(roc0, ap_radius0, thickness0, glass0);
    add_surface(roc1, ap_radius1, 0, env);
}

TLens::~TLens()
{
}

unsigned int TLens::add_surface(const QSharedPointer<TCurveBase> &curve,
                                const QSharedPointer<TShapeBase> &shape,
                                double thickness,
                                const QSharedPointer<TGlass> &glass)
{
    assert(thickness >= 0.);

    TOpticalSurface &s = _surfaces.create(Math::VectorPair3(0, 0, _last_pos),
                                         curve, shape, _next_mat, glass);

    _next_mat = glass;
    _last_pos += thickness;
    QSharedPointer<TElement> ptr(&s);

    TContainer::add(ptr);
    return _surfaces.size() - 1;
}

unsigned int TLens::add_surface(double roc, double radius,
                                double thickness,
                                const QSharedPointer<TGlass> &glass)
{
    TCurveBase* curve;;

    if (roc == 0.)
        curve = &flat;
    else
        curve = new TSphere(roc);

    TDisk* disk = new TDisk(radius);

    QSharedPointer<TCurveBase> pCurve(curve);
    QSharedPointer<TDisk> pDisk(disk);

    return add_surface(pCurve, pDisk, thickness, glass);
}

void TLens::add_stop(const QSharedPointer<TShapeBase> &shape, double thickness)
{
    if (_stop.isNull())
        throw Error("Can not add more than one stop per TLens");

    TStop* stop = new TStop(Math::VectorPair3(0, 0, _last_pos), shape);

    _stop.reset(stop);

    _last_pos += thickness;

    TContainer::add(_stop);
}

void TLens::add_stop(double radius, double thickness)
{
    TDisk* disk = new TDisk(radius);
    QSharedPointer<TDisk> pDisk(disk);

    return add_stop(pDisk, thickness);
}

double TLens::get_thickness(unsigned int index) const
{
    double z = index + 1 == _surfaces.size()
            ? _last_pos : _surfaces.at(index + 1).get_local_position().z();

    return z - _surfaces.at(index).get_local_position().z();
}

void TLens::set_thickness(double thickness, unsigned int index)
{
    double diff = thickness - get_thickness(index);

    for (unsigned int i = index; i < _surfaces.size(); i++)
    {
        Math::Vector3 p = _surfaces[i].get_local_position();
        p.z() += diff;
        _surfaces[i].set_local_position(p);
    }

    _last_pos += diff;
}

void TLens::set_left_material(const QSharedPointer<TGlass> &m)
{
    _surfaces.front().set_material(0, m);
}

void TLens::set_right_material(const QSharedPointer<TGlass> &m)
{
    _surfaces.back().set_material(1, m);
}

void TLens::set_glass_material(const QSharedPointer<TGlass> &m,
                               unsigned int index)
{
    _surfaces.at(index+1).set_material(0, m);
    _surfaces.at(index).set_material(1, m);
}

void TLens::set_left_curve(const QSharedPointer<TCurveBase> &c)
{
    _surfaces.front().set_curve(c);
}

void TLens::set_right_curve(const QSharedPointer<TCurveBase> &c)
{
    _surfaces.back().set_curve(c);
}

void TLens::set_curve(const QSharedPointer<TCurveBase> &c, unsigned int index)
{
    _surfaces.at(index).set_curve(c);
}

void TLens::set_shape(const QSharedPointer<TShapeBase> &s)
{
    for (unsigned int i = 0; i < _surfaces.size(); i++)
        _surfaces[i].set_shape(s);
}

void TLens::set_shape(const QSharedPointer<TShapeBase> &s, unsigned int index)
{
    _surfaces.at(index).set_shape(s);
}

Math::VectorPair3 TLens::get_exit_plane() const
{
    return get_transform().transform_line(Math::VectorPair3(0., 0., _last_pos));
}

void TLens::draw_3d_e(TRenderer &r, const TElement *ref) const
{
    // surfaces
    TGroup::draw_3d_e(r, ref);

#if 0
    // FIXME check / broken code

    // draw TLens contours
    for (unsigned int i = 0; i < _surfaces.size() - 1; i++)
    {
        const TSurface & left = _surfaces[i];
        const TSurface & right = _surfaces[i+1];

        const TShapeBase &s = left.get_shape();
        TRgb color = left.get_color(r);

        if (&s != &right.get_shape())
            continue;

        for (unsigned int i = 0; i < s.get_contour_count(); i++)
        {
            // get 2d contour points

            std::vector<Math::Vector2 > contour;
            delegate_push<typeof(contour)> contour_push(contour);
            s.get_contour(i, contour_push, r.get_feature_size());

            unsigned int j;
            unsigned int count = contour.size();

            if (count < 2)
                continue;

            Math::Vector3 pr[count];
            Math::Vector3 pl[count];

            // compute 3d contour points

            const Math::Transform<3> &rtr = right.get_global_transform();
            const Math::Transform<3> &ltr = left.get_global_transform();

            for (j = 0; j < count; j++)
            {
                pr[j] = rtr.transform(Math::Vector3(contour[j], right.get_curve().sagitta(contour[j])));
                pl[j] = ltr.transform(Math::Vector3(contour[j], left.get_curve().sagitta(contour[j])));
            }

            // compute normals and draw

            Math::Vector3 prevn(Math::Triangle<3>(pr[count - 1], pl[count - 1], pl[0]).normal());
            Math::Triangle<3> cur(pr[0], pl[0], pl[1]);
            Math::Vector3 curn(cur.normal());

            for (j = 0; j < count; j++)
            {
                Math::Triangle<3> next(pr[(j+1) % count], pl[(j+1) % count], pl[(j+2) % count]);
                Math::Vector3 nextn(next.normal());

                Math::Vector3 n1((prevn + curn) * 0.5);
                Math::Vector3 n2((curn + nextn) * 0.5);

                r.draw_triangle(cur, Math::Triangle<3>(n1, n1, n2), color);
                r.draw_triangle(Math::Triangle<3>(pl[(j+1) % count], pr[(j+1) % count], pr[j]),
                        Math::Triangle<3>(n2, n2, n1), color);

                prevn = curn;
                cur = next;
                curn = nextn;
            }
        }
    }
#endif
}

void TLens::draw_2d_edge(TRenderer &r, const TSurface &left, double l_y,
                         const TSurface &right, double r_y, LensEdge type,
                         const TElement *ref) const
{
    const Math::Vector3 l3(0., l_y, left.get_curve()->sagitta(Math::Vector2(0., l_y)));
    const Math::Vector2 l2(left.get_transform_to(ref).transform(l3).project_zy());

    const Math::Vector3 r3(0., r_y, right.get_curve()->sagitta(Math::Vector2(0., r_y)));
    const Math::Vector2 r2(right.get_transform_to(ref).transform(r3).project_zy());

    switch (type)
    {
    case StraightEdge: {
        if (fabs(l2.y() - r2.y()) > 1e-6)
        {
            double m;

            if (fabs(l2.y()) > fabs(r2.y()))
            {
                m = l2.y();
                r.draw_segment(Math::VectorPair2(r2.x(), m, r2.x(), r2.y()), left.get_color(r));
            }
            else
            {
                m = r2.y();
                r.draw_segment(Math::VectorPair2(l2.x(), m, l2.x(), l2.y()), left.get_color(r));
            }

            r.draw_segment(Math::VectorPair2(l2.x(), m, r2.x(), m), left.get_color(r));

            break;
        }
    }

    case SlopeEdge:
        r.draw_segment(l2, r2, left.get_color(r));
        break;
    }
}

#define GRP_BEGIN do { if (!grp) { r.group_begin(); grp = true; } } while (0)
#define GRP_END   do { if (grp) { r.group_end(); grp = false; } } while (0)

void TLens::draw_2d_e(TRenderer &r, const TElement *ref) const
{
    bool grp = false;

    if (!_stop.isNull())
    {
//        _stop.staticcast<TElement>()->draw_2d_e(r, ref);
       _stop.staticCast<TElement>()->draw_2d_e(r, ref);
    }
    else
        return;

//    if (_surfaces.empty())
//        return;

    const TOpticalSurface &first = _surfaces[0];
    if (&first.get_material(1) != &first.get_material(0))
    {
        GRP_BEGIN;
        static_cast<const TElement&>(first).draw_2d_e(r, ref);
    }

    for (unsigned int i = 0; i < _surfaces.size() - 1; i++)
    {
        const TOpticalSurface &left = _surfaces[i];
        const TOpticalSurface &right = _surfaces[i+1];

        if (!dynamic_cast<const TDielectricBase*>(&left.get_material(1)))
        {
            GRP_END;
        }
        else
        {
            // draw outter edges
            double left_top_edge =  left.get_shape()->get_outter_radius(Math::vector2_01);
            double left_bot_edge =  -left.get_shape()->get_outter_radius(-Math::vector2_01);
            double right_top_edge = right.get_shape()->get_outter_radius(Math::vector2_01);
            double right_bot_edge = -right.get_shape()->get_outter_radius(-Math::vector2_01);

            draw_2d_edge(r, left, left_top_edge, right, right_top_edge, StraightEdge, ref);
            draw_2d_edge(r, left, left_bot_edge, right, right_bot_edge, StraightEdge, ref);

            // draw hole edges if not coincident
            double left_top_hole =  left.get_shape()->get_hole_radius(Math::vector2_01);
            double left_bot_hole =  -left.get_shape()->get_hole_radius(-Math::vector2_01);
            double right_top_hole = right.get_shape()->get_hole_radius(Math::vector2_01);
            double right_bot_hole = -right.get_shape()->get_hole_radius(-Math::vector2_01);

            if (fabs(left_bot_hole - left_top_hole) > 1e-6 ||
                    fabs(right_bot_hole - right_top_hole) > 1e-6)
            {
                draw_2d_edge(r, left, left_top_hole, right, right_top_hole, SlopeEdge, ref);
                draw_2d_edge(r, left, left_bot_hole, right, right_bot_hole, SlopeEdge, ref);
            }
        }

        if (&right.get_material(1) != &right.get_material(0))
        {
            GRP_BEGIN;
            static_cast<const TElement&>(right).draw_2d_e(r, ref);
        }
    }

    GRP_END;
}

const TOpticalSurface & TLens::get_surface(unsigned int index) const
{
    return _surfaces.at(index);
}

TOpticalSurface & TLens::get_surface(unsigned int index)
{
    return _surfaces.at(index);
}

const TOpticalSurface & TLens::get_right_surface() const
{
    return _surfaces.back();
}

TOpticalSurface & TLens::get_right_surface()
{
    return _surfaces.back();
}

const TOpticalSurface & TLens::get_left_surface() const
{
    return _surfaces.front();
}

TOpticalSurface & TLens::get_left_surface()
{
    return _surfaces.front();
}


