#ifndef TARRAY_H
#define TARRAY_H

#include <QSharedPointer>

#include "tcurvebase.h"

class TArray : public TCurveBase
{
public:

  /** Specify tessellation pattern used by @ref Array class */
  enum pattern_e
    {
      Square,
      SquareCenter,
      Hexagonal,
    };

  TArray(TCurveBase &curve, double pitch, enum pattern_e p = Square);

  /** @override */
  double sagitta(const Math::Vector2 & xy) const;
  /** @override */
  void derivative(const Math::Vector2 & xy, Math::Vector2 & dxdy) const;

private:

  typedef Math::Vector2 (TArray::*transform_t)(const Math::Vector2 &v) const;

  Math::Vector2 transform_square(const Math::Vector2 &v) const;
  Math::Vector2 transform_square_center(const Math::Vector2 &v) const;

  Math::Vector2 transform_hexagonal(const Math::Vector2 &v) const;
  Math::Vector2 transform_hexagonal_center(const Math::Vector2 &v) const;

  QSharedPointer<TCurveBase> _curve;
  double _pitch;
  transform_t _transform;
};

#endif // TARRAY_H
