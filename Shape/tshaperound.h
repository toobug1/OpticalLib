#ifndef TSHAROROUND_H
#define TSHAROROUND_H

#include "Math/Triangle"
#include "Trace/tdistribution.h"
#include "Math/Vector"

using namespace _Goptical;

template <class X, bool hole> class TShapeRound : public X
{
public:
    /** @override */
    unsigned int get_contour_count() const;
    /** @override */
    void get_pattern(const Math::Vector2::put_delegate_t  &v, const TDistribution &d,
                     bool unobstructed) const;
    /** @override */
    void get_contour(unsigned int contour, const Math::Vector2::put_delegate_t  &f,
                     double resolution) const;
    /** @override */
    void get_triangles(const Math::Triangle<2>::put_delegate_t  &f, double resolution) const;

private:
    double get_radial_step(double resolution) const;
};

template <class X, bool hole>
unsigned int TShapeRound<X, hole>::get_contour_count() const
{
    return get_contour_count();
}

template <class X, bool hole>
void TShapeRound<X, hole>::get_contour(unsigned int contour,
                                       const Math::Vector2::put_delegate_t  &f,
                                       double resolution) const
{
    get_contour(contour, f, resolution);
}

template <class X, bool hole>
void TShapeRound<X, hole>::get_triangles(const Math::Triangle<2>::put_delegate_t  &f,
                                         double resolution) const
{
    get_triangles(f, resolution);
}

template <class X, bool hole>
void TShapeRound<X, hole>::get_pattern(const Math::Vector2::put_delegate_t  &v,
                                       const TDistribution &d,
                                       bool unobstructed) const
{
    get_pattern(v, d, unobstructed);
}

#endif // TSHAROROUND_H
