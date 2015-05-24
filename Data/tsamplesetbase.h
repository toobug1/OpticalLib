#ifndef TSAMPLESETBASE_H
#define TSAMPLESETBASE_H

//#include <vector>

#include "tset1d.h"


class TSampleSetBase : public TSet1d
{
public:
    TSampleSetBase();
    ~TSampleSetBase();

    /** Set y value and its derivative at index x */
    void set_value(unsigned int x, double y, double d = 0.0);

    /** Set y value and its derivative at nearest x value */
    void set_value_near(double x, double y, double d = 0.0);

    /** Clear all data */
    void clear();

    /** Set number of samples */
    void resize(unsigned int n);

    /** Set x origin and step values */
    void set_metrics(double origin, double step);

    /** Get step size between consecutive x values */
    double get_step() const;

    /** Get x value of first sample */
    double get_origin() const;

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
    /* y and prescribed first derivative values */
    struct entry_s
    {
        double y, d;
    };

    /** find lower bound index of interval containing value */
    unsigned int get_interval(double x) const;
    /** find nearest value index */
    unsigned int get_nearest(double x) const;

    virtual void invalidate() = 0;

    double get_x_interval(unsigned int x) const;
    double get_x_interval(unsigned int x1, unsigned int x2) const;

    double            _origin, _step;
    std::vector<struct entry_s> _data;
};

#endif // TSAMPLESETBASE_H
