#ifndef TSPLINE_H
#define TSPLINE_H

#include "trotational.h"

class TSpline : public TRotational
{
public:
  /** Create an empty spline curve with no defined point */
  TSpline();
  ~TSpline();

  /** Get sagitta/derivative data container */
//  inline const Data::DiscreteSet & get_data() const;

//  /** get sagitta/derivative data container */
//  inline Data::DiscreteSet & get_data();

  /** Clear all points and fit to an other rotationally symmetric curve.
      @param c Curve to fit
      @param radius Maximum radius where curve is defined
      @param points Number of sample points
  */
  void fit(const TRotational &c, double radius, unsigned int points);

  inline double sagitta(double r) const;
  inline double derivative(double r) const;

protected:
//  Data::DiscreteSet _data;

  // TODO
};

#endif // TSPLINE_H
