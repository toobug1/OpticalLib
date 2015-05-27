#include "tdiscreteset.h"
#include "Error"

#include <assert.h>

void TDiscreteSetBase::reserve(size_t n)
{
    _data.reserve(n);
}

unsigned int TDiscreteSetBase::get_count() const
{
    return _data.size();
}

double TDiscreteSetBase::get_x_value(unsigned int n) const
{
    assert(n < _data.size());
    return _data[n].x;
}

double TDiscreteSetBase::get_y_value(unsigned int n) const
{
    assert(n < _data.size());
    return _data[n].y;
}

double TDiscreteSetBase::get_d_value(unsigned int n) const
{
    assert(n < _data.size());
    return _data[n].d;
}

double & TDiscreteSetBase::get_y_value(unsigned int n)
{
    assert(n < _data.size());
    invalidate();
    return _data[n].y;
}

double & TDiscreteSetBase::get_d_value(unsigned int n)
{
    assert(n < _data.size());
    invalidate();
    return _data[n].d;
}

double TDiscreteSetBase::get_x_interval(unsigned int x) const
{
    return _data[x + 1].x - _data[x].x;
}

double TDiscreteSetBase::get_x_interval(unsigned int x1, unsigned int x2) const
{
    return _data[x2].x - _data[x1].x;
}

TDiscreteSetBase::TDiscreteSetBase()
    : TSet1d(),
      _data()
{
}

TDiscreteSetBase::~TDiscreteSetBase()
{
}

unsigned int TDiscreteSetBase::get_interval(double x) const
{
    int min_idx = 0;
    int max_idx = _data.size() + 1;

    while (max_idx - min_idx > 1)
    {
        unsigned int p = (max_idx + min_idx) / 2;

        if (x >= _data[p - 1].x)
            min_idx = p;
        else
            max_idx = p;
    }

    return min_idx;
}

unsigned int TDiscreteSetBase::get_nearest(double x) const
{
    int min_idx = 0;
    int max_idx = _data.size();

    while (max_idx - min_idx > 1)
    {
        unsigned int p = (max_idx + min_idx) / 2;

        if (x + x >= _data[p - 1].x + _data[p].x)
            min_idx = p;
        else
            max_idx = p;
    }

    return min_idx;
}

void TDiscreteSetBase::add_data(double x, double y, double d)
{
    const struct entry_s e = { x, y, d };

    _version++;

    unsigned int di = get_interval(x);

    if (di && (_data[di - 1].x == x))
        _data[di - 1] = e;
    else
        _data.insert(_data.begin() + di, e);

    invalidate();
}

void TDiscreteSetBase::clear()
{
    _data.clear();
    _version++;
    invalidate();
}

Math::range_t TDiscreteSetBase::get_x_range() const
{
    if (_data.empty())
        throw Error("_data set contains no _data");

    return Math::range_t(_data.front().x, _data.back().x);
}

