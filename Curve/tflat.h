#ifndef TFLAT_H
#define TFLAT_H

#include "trotational.h"


class TFlat : public TRotational
{
public:
  /** Creates a flat curve */
  TFlat();

  bool intersect(Math::Vector3 &point, const Math::VectorPair3 &ray) const;
  void normal(Math::Vector3 &normal, const Math::Vector3 &point) const;

  double sagitta(double r) const;
  double derivative(double r) const;
};

extern TFlat flat;

#endif // TFLAT_H
