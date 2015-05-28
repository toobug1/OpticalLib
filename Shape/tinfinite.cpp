#include "tinfinite.h"

#include "Math/Vector"
#include "Math/VectorPair"

TInfinite::TInfinite()
{
}

double TInfinite::max_radius() const
{
    return 0;
}

double TInfinite::min_radius() const
{
    return 0;
}

bool TInfinite::inside(const Math::Vector2 &point) const
{
    return true;
}

Math::VectorPair2 TInfinite::get_bounding_box() const
{
    return Math::vector2_pair_00;
}

void TInfinite::get_pattern(const Math::Vector2::put_delegate_t  &f, const TDistribution &d, bool unobstructed) const
{
    throw Error("can not distribute rays accross an TInfinite surface shape");
}

unsigned int TInfinite::get_contour_count() const
{
    return 0;
}

void TInfinite::get_contour(unsigned int contour, const Math::Vector2::put_delegate_t  &f, double resolution) const
{
}

void TInfinite::get_triangles(const Math::Triangle<2>::put_delegate_t  &t, double resolution) const
{
}

double TInfinite::get_outter_radius(const Math::Vector2 &dir) const
{
    return 0;
}


