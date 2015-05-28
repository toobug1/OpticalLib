#include "tshapebase.h"
#include "Error"


#include <time.h>

#define ADD_PATTERN_POINT(v)                    \
{                                           \
    Math::Vector2 v_(v);                    \
    if (unobstructed || inside(v_))           \
    f(v_);                                  \
    }

void TShapeBase::get_pattern(const Math::Vector2::put_delegate_t  &f,
                       const TDistribution &d,
                       bool unobstructed) const
{
    static const double epsilon = 1e-8;
    // FIXME use bounding box instead of max radius
    const double      tr = max_radius() * d.get_scaling();
    const double      step = tr / d.get_radial_density();

    Trace::Pattern p = d.get_pattern();

    switch (p)
    {
    case Trace::MeridionalDist: {

        double        r = tr;

        ADD_PATTERN_POINT(Math::vector2_0);

        for (unsigned int i = 0; i < d.get_radial_density(); i++)
        {
            ADD_PATTERN_POINT(Math::Vector2(0, r));
            ADD_PATTERN_POINT(Math::Vector2(0, -r));
            r -= step;
        }
        break;
    }

    case Trace::SagittalDist: {

        double r = tr;

        ADD_PATTERN_POINT(Math::Vector2(0, 0));

        for (unsigned int i = 0; i < d.get_radial_density(); i++)
        {
            ADD_PATTERN_POINT(Math::Vector2(r, 0));
            ADD_PATTERN_POINT(Math::Vector2(-r, 0));
            r -= step;
        }
        break;
    }

    case Trace::CrossDist: {

        double r = step;

        ADD_PATTERN_POINT(Math::Vector2(0, 0));

        for (unsigned int i = 0; i < d.get_radial_density(); i++)
        {
            ADD_PATTERN_POINT(Math::Vector2(0, r));
            ADD_PATTERN_POINT(Math::Vector2(r, 0));
            ADD_PATTERN_POINT(Math::Vector2(0, -r));
            ADD_PATTERN_POINT(Math::Vector2(-r, 0));
            r += step;
        }
        break;
    }

    case Trace::RandomDist: {

        double x, y;
        long long seed=1;
        long long m =0x100000000LL;
        long long c=0xB16;
        long long a=0x5DEECE66DLL;
        double myrand48;
        srand(time(NULL));
        seed  = (((long long int)(time(NULL))) << 16) | rand();
        seed = (a * seed + c) & 0xFFFFFFFFFFFFLL;
        unsigned int x_seed = seed >> 16;
        myrand48= ((double)x_seed / (double)m);
        for (x = -tr; x < tr; x += step)
        {
            double ybound = sqrt(Math::square(tr) - Math::square(x));

            for (y = -ybound; y < ybound; y += step)
            {
                ADD_PATTERN_POINT(Math::Vector2(x + (myrand48 - .5) * step,
                                                y + (myrand48 - .5) * step));
            }

        }
        break;
    }

    case Trace::HexaPolarDist: {

        ADD_PATTERN_POINT(Math::Vector2(0, 0));

        for (double r = tr; r > epsilon; r -= step)
        {
            double astep = (step / r) * (M_PI / 3);

            for (double a = 0; a < 2 * M_PI - epsilon; a += astep)
                ADD_PATTERN_POINT(Math::Vector2(sin(a) * r, cos(a) * r));
        }

        break;
    };

    case Trace::SquareDist: {

        ADD_PATTERN_POINT(Math::Vector2(0, 0));

        double x, y;

        for (x = tr; x > epsilon; x -= step)
        {
            double ybound = sqrt(Math::square(tr) - Math::square(x));

            for (y = step; y < ybound; y += step)
            {
                ADD_PATTERN_POINT(Math::Vector2(x, y));
                ADD_PATTERN_POINT(Math::Vector2(x, -y));
                ADD_PATTERN_POINT(Math::Vector2(-x, y));
                ADD_PATTERN_POINT(Math::Vector2(-x, -y));
            }

            ADD_PATTERN_POINT(Math::Vector2(x, 0));
            ADD_PATTERN_POINT(Math::Vector2(-x, 0));
        }

        for (y = step; y < tr + epsilon; y += step)
        {
            ADD_PATTERN_POINT(Math::Vector2(0, y));
            ADD_PATTERN_POINT(Math::Vector2(0, -y));
        }
        break;
    }

    case Trace::DefaultDist:
    case Trace::TriangularDist: {
        static const double sqrt_3_2 = 0.86602540378443864676;
        double x, y;
        unsigned i = 1;

        for (x = step * sqrt_3_2; x < tr + epsilon; x += step * sqrt_3_2)
        {
            for (y = step / (double)i; y < tr + epsilon; y += step)
            {
                double h = hypot(x, y);

                if (h > tr)
                    break;

                ADD_PATTERN_POINT(Math::Vector2(x, y));
                ADD_PATTERN_POINT(Math::Vector2(-x, y));
                ADD_PATTERN_POINT(Math::Vector2(x, -y));
                ADD_PATTERN_POINT(Math::Vector2(-x, -y));
            }

            i ^= 3;
        }

        for (y = step / 2.0; y < tr + epsilon; y += step)
        {
            ADD_PATTERN_POINT(Math::Vector2(0, y));
            ADD_PATTERN_POINT(Math::Vector2(0, -y));
        }

        for (x = step * sqrt_3_2; x < tr + epsilon; x += step * sqrt_3_2 * 2.0)
        {
            ADD_PATTERN_POINT(Math::Vector2(x, 0));
            ADD_PATTERN_POINT(Math::Vector2(-x, 0));
        }
        break;
    }

    default:
        throw Error("distribution pattern not supported for this shape");
    }
}

double TShapeBase::get_hole_radius(const Math::Vector2 &dir) const
{
    return 0.;
}
