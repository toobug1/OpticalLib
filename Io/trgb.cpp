#include "trgb.h"

TRgb::TRgb()
{
}

TRgb::TRgb(float red, float green, float blue, float alpha)
    : r(red), g(green), b(blue), a(alpha)
{
}

bool TRgb::operator< (const TRgb &TRgb) const
{
    return (a * 1000.0 + r * 100.0 + g * 10.0 + b <
            TRgb.a * 1000.0 + TRgb.r * 100.0 + TRgb.g * 10.0 + TRgb.b);
}

TRgb TRgb::operator~ () const
{
    return TRgb(1. - r, 1. - g, 1. - b, a);
}

const TRgb rgb_black(0.0, 0.0, 0.0);
const TRgb rgb_red(1.0, 0.0, 0.0);
const TRgb rgb_green(0.0, 1.0, 0.0);
const TRgb rgb_blue(0.0, 0.0, 1.0);
const TRgb rgb_yellow(1.0, 1.0, 0.0);
const TRgb rgb_cyan(0.0, 1.0, 1.0);
const TRgb rgb_magenta(1.0, 0.0, 1.0);
const TRgb rgb_gray(0.5, 0.5, 0.5);
const TRgb rgb_white(1.0, 1.0, 1.0);

const TRgb rgb_table[] =
{
    TRgb (0.0, 0.0, 0.0),
    TRgb (1.0, 0.0, 0.0),
    TRgb (0.0, 1.0, 0.0),
    TRgb (0.0, 0.0, 1.0),
    TRgb (1.0, 1.0, 0.0),
    TRgb (0.0, 1.0, 1.0),
    TRgb (1.0, 0.0, 1.0),
    TRgb (0.5, 0.5, 0.5),
    TRgb (1.0, 1.0, 1.0),
};

