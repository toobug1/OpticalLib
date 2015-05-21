#include "tarray.h"

#include "math.h"

TArray::TArray(TCurveBase &curve, double pitch, enum pattern_e p)
    : _pitch(pitch)
{
    QSharedPointer<TCurveBase> ptr(&curve);
    _curve = ptr;

    switch (p)
    {
    case Square:
        _transform = &TArray::transform_square;
        break;

    case SquareCenter:
        _transform = &TArray::transform_square_center;
        break;

    case Hexagonal:
        _transform = &TArray::transform_hexagonal;
        break;
    }
}

static inline double mod(double x, double n)
{
    x = x - trunc(x / n) * n;
    return x < 0 ? x + n : x;
}

Math::Vector2 TArray::transform_square(const Math::Vector2 &v) const
{
    double h = _pitch / 2.0;

    return Math::Vector2(mod(v.x(), _pitch) - h,
                         mod(v.y(), _pitch) - h);
}

Math::Vector2 TArray::transform_square_center(const Math::Vector2 &v) const
{
    double h = _pitch / 2.0;

    return Math::Vector2(mod(v.x() - h, _pitch) - h,
                         mod(v.y() - h, _pitch) - h);
}

Math::Vector2 TArray::transform_hexagonal(const Math::Vector2 &v) const
{
    double pitch = _pitch / 2.0;
    double h1 = pitch * 0.86602540378443864676;
    double h2 = pitch * 1.73205080756887729352;
    double h3 = pitch * 1.5;
    double x, y = mod(v.y(), h3 * 2.0);

    if (y > h3)
    {
        y -= h3;
        x = mod(v.x() - h1, h2);
    }
    else
    {
        x = mod(v.x(), h2);
    }

    if (y > pitch && (h3 - y) * h1 < fabs((h1 - x) * pitch / 2.0))
    {
        y -= h3;
        x = x > h1 ? x - h1 : x + h1;
    }

    return Math::Vector2(x - h1, y - pitch / 2.0);
}

double TArray::sagitta(const Math::Vector2 & xy) const
{
    return _curve->sagitta((this->*_transform)(xy));
}

void TArray::derivative(const Math::Vector2 & xy, Math::Vector2 & dxdy) const
{
    _curve->derivative((this->*_transform)(xy), dxdy);
}
