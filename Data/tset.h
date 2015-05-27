#ifndef TSET_H
#define TSET_H

#include <iostream>

#include "common.hh"

using namespace _Goptical;
using namespace _Goptical::Data;

class TSet
{

    friend std::ostream & operator<<(std::ostream &o, const TSet &s);

public:
    virtual ~TSet();

    /** Get total number of dimensions */
    virtual unsigned int get_dimensions() const = 0;

    /** Get total number of data stored for dimension n in data set */
    virtual unsigned int get_count(unsigned int dim = 0) const = 0;

    /** Get data stored at position n on dimension dim in data set */
    virtual double get_x_value(unsigned int n, unsigned int dim = 0) const = 0;

    /** Get y data stored at position (x0, x1, ...) in data set */
    virtual double get_y_value(const unsigned int x[]) const = 0;

    /** Interpolate y value corresponding to given x value(s) in data set. */
    virtual double interpolate(const double x[]) const = 0;

    /** Interpolate y value corresponding to given x value in data
        set. Data may be differentiated several times along the requested dimension.
        @param deriv Differentiation count, 0 means y value, 1 means 1st derivative...
        @param dim Differentiation dimension
    */
    virtual double interpolate(const double x[], unsigned int deriv, unsigned int dim) const = 0;

    /** Get minimal and maximal x values on dimension n found in data set */
    virtual Math::range_t get_x_range(unsigned int dim = 0) const = 0;

    /** Get minimal and maximal y values found in data set */
    virtual Math::range_t get_y_range() const;

    /** Select interpolation method */
    virtual void set_interpolation(Interpolation i) = 0;

    /** Get current interpolation method */
    Interpolation get_interpolation();

    // FIXME dataset version number
    /** Return version number which is incremented on each data set change/clear */
    unsigned int get_version() const;

protected:
    TSet();

    unsigned int      _version;
    Interpolation     _interpolation;
};

#endif // TSET_H
