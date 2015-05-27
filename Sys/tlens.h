#ifndef TLENS_H
#define TLENS_H

#include <QSharedPointer>

#include "common.hh"
#include "Material/tglass.h"
#include "tgroup.h"
#include "Curve/tcurvebase.h"
#include "Shape/tshapebase.h"
#include "tsurface.h"
#include "vector_pool"
#include "topticalsurface.h"
#include "tstop.h"

using namespace _Goptical::Sys;

class TLens : public TGroup
{
public:
    /** Create an empty lens. Surfaces can be added with the @mref
      add_surface functions. */
    TLens(const Math::VectorPair3 &p, double offset = 0.,
          const QSharedPointer<TGlass> &env = QSharedPointer<const TGlass>(&none));

    /** Create a lens at given position with given thickness,
      shapes, curvatures and glass. @see __add_surface1__ */
    TLens(const Math::VectorPair3 &p,
          const QSharedPointer<TCurveBase> &curve0,
          const QSharedPointer<TShapeBase> &shape0,
          const QSharedPointer<TCurveBase> &curve1,
          const QSharedPointer<TShapeBase> &shape1,
          double thickness0,
          const QSharedPointer<TGlass> &glass0,
          const QSharedPointer<TGlass> &env = QSharedPointer<const TGlass>(&none));

    /** Create a circular lens with flat or spherical surfaces at
      given position with given thickness, radius of curvature,
      aperture radius and glass. @see __add_surface2__ */
    TLens(const Math::VectorPair3 &p,
          double roc0, double ap_radius0,
          double roc1, double ap_radius1,
          double thickness,
          const QSharedPointer<TGlass> &glass0,
          const QSharedPointer<TGlass> &env = QSharedPointer<const TGlass>(&none));

    virtual ~TLens();

    /** @alias add_surface1
      Add an optical surface with given curve, shape, thickness and material.
  */
    unsigned int add_surface(const QSharedPointer<TCurveBase> &curve,
                             const QSharedPointer<TShapeBase> &shape,
                             double thickness,
                             const QSharedPointer<TGlass> &glass =
                                                 QSharedPointer<const TGlass>(&none));

    /** @alias add_surface2
      Add a spherical or flat optical surface with circular aperture.

      @param roc spherical radius of curvature. 0 means flat curve.
      @param ap_radius circular aperture radius.
      @param thickness distance to the next surface or exit plane. @see get_exit_plane.
      @param glass material between added surface and next surface.
  */
    unsigned int add_surface(double roc,
                             double ap_radius,
                             double thickness = 0.,
                             const QSharedPointer<TGlass> &glass =
                                                QSharedPointer<const TGlass>(&none));

    /** @alias add_stop1
      Add an aperture stop with given, shape and thickness.
  */
    void add_stop(const QSharedPointer<TShapeBase> &shape,
                  double thickness);

    /** @alias add_stop2
      Add an aperture stop with circular aperture.

      @param radius circular aperture radius.
      @param thickness distance to the next stop or exit plane. @see get_exit_plane.
  */
    void add_stop(double ap_radius, double thickness);

    /** Adjust thickness between two surfaces */
    void set_thickness(double thickness, unsigned int index = 0);
    /** Get thickness between two surfaces */
    double get_thickness(unsigned int index = 0) const;

    /** Set glass material */
    void set_glass_material(const QSharedPointer<TGlass> &m,
                            unsigned int index = 0);

    /** Set left material */
    void set_left_material(const QSharedPointer<TGlass> &m);
    /** Set left material */
    void set_right_material(const QSharedPointer<TGlass> &m);

    /** Set left curve */
    void set_left_curve(const QSharedPointer<TCurveBase> &c);
    /** Set right curve */
    void set_right_curve(const QSharedPointer<TCurveBase> &c);

    /** Set curve of given surface index */
    void set_curve(const QSharedPointer<TCurveBase> &c, unsigned int index);

    /** Set lens shape (all surfaces) */
    void set_shape(const QSharedPointer<TShapeBase> &s);

    /** Set lens shape of given surface index */
    void set_shape(const QSharedPointer<TShapeBase> &s, unsigned int index);

    /** Get a reference to optical surface at given index */
    inline const TOpticalSurface & get_surface(unsigned int index) const;
    /** Get a reference to optical surface at given index */
    inline TOpticalSurface & get_surface(unsigned int index);

    /** Get a reference to right optical surface element */
    inline const TOpticalSurface & get_right_surface() const;
    /** Get a reference to right optical surface element */
    inline TOpticalSurface & get_right_surface();

    /** Get a reference to left optical surface element */
    inline const TOpticalSurface & get_left_surface() const;
    /** Get a reference to left optical surface element */
    inline TOpticalSurface & get_left_surface();

    /** Get plane of last surface + thickness z offset */
    Math::VectorPair3 get_exit_plane() const;

private:

    /** prevent use of @ref Container::add */
    inline void add(const QSharedPointer<TElement> &e);
    /** prevent use of @ref Container::remove */
    inline void remove(TElement &e);

    /** @override */
    void draw_2d_e(Io::Renderer &r, const TElement *ref) const;
    /** @override */
    void draw_3d_e(Io::Renderer &r, const TElement *ref) const;

    void draw_2d_edge(Io::Renderer &r, const TSurface &left, double l_y,
                      const TSurface &right, double r_y, LensEdge type,
                      const TElement *ref) const;

    bool check_thickness(double thickness);

    double _last_pos;

    // preallocated static storage for 8 surface
    vector_pool<TOpticalSurface, 8>::block_type _surfaces_storage;
    vector_pool<TOpticalSurface, 8> _surfaces;

    QSharedPointer<TStop> _stop;
    QSharedPointer<TGlass> _next_mat;
};

#endif // TLENS_H
