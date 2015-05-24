#ifndef TSET1D_H
#define TSET1D_H

#include "tset.h"


/*
   @short Base class for 1d y = f(x) numerical data set
   @header Goptical/Data/Set1d
   @module {Core}
*/

class TSet1d : public TSet
{
public:
    virtual ~TSet1d();

    /** Get total number of data stored in data set */
    virtual unsigned int get_count() const = 0;

    /** Get x data at index n in data set */
    virtual double get_x_value(unsigned int n) const = 0;
    /** Get y data stored at index n in data set */
    virtual double get_y_value(unsigned int n) const = 0;
    /** Get modifiable reference to y data stored at index n in data set */
    virtual double & get_y_value(unsigned int n) = 0;

    /** Interpolate y value corresponding to given x value in data set. */
    virtual double interpolate(const double x) const = 0;
    /** Interpolate y value corresponding to given x value in data
      set. Data may be differentiated several times.
      @param deriv Differentiation count, 0 means y value, 1 means 1st derivative...
  */
    virtual double interpolate(const double x, unsigned int deriv) const = 0;

    /** Get minimal and maximal x values on found in data set */
    virtual Math::range_t get_x_range() const = 0;

    // inherited from Set
    unsigned int get_dimensions() const;
    unsigned int get_count(unsigned int dim) const;
    double get_x_value(unsigned int n, unsigned int dim) const;
    double get_y_value(const unsigned int x[]) const;
    double interpolate(const double x[]) const;
    double interpolate(const double x[], unsigned int deriv, unsigned int dimension) const;
    Math::range_t get_x_range(unsigned int dimension) const;

protected:
    TSet1d();
};

#endif // TSET1D_H
