#ifndef TDISCRETESETBASE_H
#define TDISCRETESETBASE_H

#include "tset1d.h"
#include "tinterpolate1d.h"

class TDiscreteSetBase : public TSet1d
{

public:

    TDiscreteSetBase();
    ~TDiscreteSetBase();

    /** Insert data pair in data set. If a pair with the same x
      value exists, it will be replaced by the new
      value. Derivative value may be provided as well. */
    void add_data(double x, double y, double yp = 0.0);

    /** Reserve data entries */
    void reserve(size_t n);

    /** Clear all data */
    void clear();

    /** Get stored derivative value at index x */
    double get_d_value(unsigned int x) const;
    /** Get modifiable reference to stored derivative value at index x */
    double & get_d_value(unsigned int x);

    // inherited from Set1d
    unsigned int get_count() const;
    double get_x_value(unsigned int x) const;
    double get_y_value(unsigned int x) const;
    double & get_y_value(unsigned int x);
    Math::range_t get_x_range() const;

protected:
    /** x, y and user prescribed first derivative values */
    struct entry_s
    {
        double x, y, d;
    };

    /** find lower bound index of interval containing value */
    unsigned int get_interval(double x) const;
    /** find nearest value index */
    unsigned int get_nearest(double x) const;

    virtual void invalidate() = 0;

    double get_x_interval(unsigned int x) const;
    double get_x_interval(unsigned int x1, unsigned int x2) const;

    std::vector<struct entry_s>       _data;
};

/**
    @short 1d numerical data set with interpolation
    @header Goptical/Data/DiscreteSet
    @module {Core}
    @main

    This class provides a numerical data container
    where value pairs can be defined at arbitrary positions (x, y).

    Severals interpolation algorithms are available to guess
    values between defined knots, see @ref Interpolation.

    @see SampleSet
*/
class TDiscreteSet : public TInterpolate1d<TDiscreteSetBase>
{
public:

    TDiscreteSet() : TInterpolate1d<TDiscreteSetBase>() { }
    void invalidate() {_data.clear();}
};

#endif // TDISCRETESETBASE_H
