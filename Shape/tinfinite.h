#ifndef TINFINITE_H
#define TINFINITE_H

#include "tshapebase.h"
#include "Trace/tdistribution.h"

class TInfinite : public TShapeBase
{
public:
  /** Create an infinite shape */
  TInfinite();

  /** @override */
  double max_radius() const;
  /** @override */
  double min_radius() const;
  /** @override */
  double get_outter_radius(const Math::Vector2 &dir) const;
  /** @override */
  Math::VectorPair2 get_bounding_box() const;
  /** @override */
  bool inside(const Math::Vector2 &point) const;
  /** @override */
  void get_pattern(const Math::Vector2::put_delegate_t  &v,
                   const TDistribution &d,
                   bool unobstructed) const;
  /** @override */
  unsigned int get_contour_count() const;
  /** @override */
  void get_contour(unsigned int contour,
                   const Math::Vector2::put_delegate_t  &f,
                   double resolution) const;
  /** @override */
  void get_triangles(const Math::Triangle<2>::put_delegate_t  &f,
                     double resolution) const;
};

#endif // TINFINITE_H
