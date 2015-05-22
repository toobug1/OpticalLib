#include "tsamplesetbase.h"

#include "Error"
#include "assert.h"

void TSampleSetBase::set_metrics(double origin, double step)
{
    _origin = origin;
    _step = step;
    invalidate();
}

unsigned int TSampleSetBase::get_count() const
{
    return _data.size();
}

double TSampleSetBase::get_x_value(unsigned int n) const
{
    return _origin + (double)n * _step;
}

double TSampleSetBase::get_y_value(unsigned int n) const
{
    return _data[n].y;
}

double & TSampleSetBase::get_y_value(unsigned int n)
{
    invalidate();
    return _data[n].y;
}

double TSampleSetBase::get_d_value(unsigned int n) const
{
    return _data[n].d;
}

double & TSampleSetBase::get_d_value(unsigned int n)
{
    invalidate();
    return _data[n].d;
}

double TSampleSetBase::get_step() const
{
    return _step;
}

double TSampleSetBase::get_origin() const
{
    return _origin;
}

double TSampleSetBase::get_x_interval(unsigned int x) const
{
    return _step;
}

double TSampleSetBase::get_x_interval(unsigned int x1, unsigned int x2) const
{
    return _step * (double)(x2 - x1);
}

TSampleSetBase::TSampleSetBase()
    : TSet1d(),
      _origin(0),
      _step(1.0),
      _data()
{
}

TSampleSetBase::~TSampleSetBase()
{
}

Math::range_t TSampleSetBase::get_x_range() const
{
    if (_data.empty())
        throw Error("data set contains no data");

    return Math::range_t(_origin, _origin + _step * (_data.size() - 1));
}

unsigned int TSampleSetBase::get_interval(double x) const
{
    int n = (int)floor((x - _origin) / _step);

    if (n < 0)
        return 0;
    else if (n >= (int)_data.size())
        return _data.size();
    else
        return n + 1;
}

unsigned int TSampleSetBase::get_nearest(double x) const
{
    int n = (int)round((x - _origin) / _step);

    if (n < 0)
        return 0;
    else if (n >= (int)_data.size())
        return _data.size() - 1;
    else
        return n;
}

void TSampleSetBase::set_value(unsigned int x, double y, double d)
{
    struct entry_s ze = { 0.0, 0.0 };

    if (x >= _data.size())
        _data.resize(x + 1, ze);

    struct entry_s e = { y, d };

    _data[x] = e;
    invalidate();
}

void TSampleSetBase::set_value_near(double x, double y, double d)
{
    assert(x >= _origin);

    struct entry_s ze = { 0.0, 0.0 };

    int n = (int)round((x - _origin) / _step);

    if (n >= (int)_data.size())
        _data.resize(n + 1, ze);

    struct entry_s e = { y, d };

    _data[n] = e;
    invalidate();
}

void TSampleSetBase::clear()
{
    _data.clear();
    invalidate();
}

void TSampleSetBase::resize(unsigned int n)
{
    struct entry_s ze = { 0.0, 0.0 };

    _data.resize(n, ze);
    invalidate();
}

