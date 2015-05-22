#include "tset.h"
#include "Error"

#include <limits>

unsigned int TSet::get_version() const
{
    return _version;
}

Interpolation TSet::get_interpolation()
{
    return _interpolation;
}

TSet::TSet()
    : _version(0)
{
}

TSet::~TSet()
{
}

Math::range_t TSet::get_y_range() const
{
    Math::range_t r(std::numeric_limits<double>::max(),
                    std::numeric_limits<double>::min());

    unsigned int d = get_dimensions();
    unsigned int x[d];
    unsigned int c[d];

    for (unsigned int i = 0; i < d; i++)
    {
        if (get_count(i) == 0)
            throw Error("data set contains no data");

        x[i] = 0;
        c[i] = get_count(i) - 1;
    }

    while (1)
    {
        double y = get_y_value(x);

        if (y < r.first)
            r.first = y;

        if (y > r.second)
            r.second = y;

        for (unsigned int i = 0; ; )
        {
            if (x[i] < c[i])
            {
                x[i]++;
                break;
            }
            else
            {
                x[i++] = 0;

                if (i == d)
                    return r;
            }
        }
    }
}


std::ostream & operator<<(std::ostream &o, const TSet &s)
{
    switch (s.get_dimensions())
    {
    case 2:
        for (unsigned int x1 = 0; x1 < s.get_count(0); x1++)
        {
            for (unsigned int x2 = 0; x2 < s.get_count(1); x2++)
            {
                unsigned int t[2];

                t[0] = x1;
                t[1] = x2;

                o << s.get_x_value(x1, 0) << " "
                  << s.get_x_value(x2, 1) << " "
                  << s.get_y_value(t) << " " << std::endl;
            }
            o << std::endl;
        }
        break;

    case 1:
        for (unsigned int x = 0; x < s.get_count(0); x++)
            o << s.get_x_value(x, 0) << " " << s.get_y_value(&x) << std::endl;
        break;
    }

    return o;
}

