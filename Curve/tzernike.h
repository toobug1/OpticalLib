#ifndef TZERNIKE_H
#define TZERNIKE_H

#include "Trace/tdistribution.h"
#include "tcurvebase.h"

class TZernike : public TCurveBase
{
    static const TDistribution default_dist;

public:
    /** Number of implemented zernike polynomials */
    static const unsigned int term_count = 36;

    /** Create a Zernike curve defined over the given circle radius.
        @param radius Zernike circle radius
        @param unit_scale Sagitta scale factor used to change units globally
    */
    TZernike(double radius, double unit_scale = 1.0);

    /** Create a Zernike curve defined over the given circle radius
        and initialize coefficients from table.
        @param radius Zernike circle radius
        @param coefs Table of Zernike coefficients starting with z0 (piston)
        @param coefs_count Number of coefficients available in the table
        @param unit_scale Sagitta scale factor used to change units globally
    */
    TZernike(double radius, double coefs[],
             unsigned int coefs_count, double unit_scale = 1.0);

    /** Set Zernike circle radius */
    void set_radius(double radius);
    /** Get Zernike circle radius */
    double get_radius() const;

    /** Set coefficient associated with zernike term n and enable
        term according to current threshold. See
        set_coefficients_threshold() */
    void set_coefficient(unsigned int n, double c);

    /** Get coefficient associated with zernike term n */
    double get_coefficient(unsigned int n) const;

    /** Set coefficients unit scale factor. default is 1 (1 mm). */
    void set_coefficients_scale(double s);

    /** Set new coefficient threshold and adjust all term states
        (enabled/disabled) depending on new threshold. Term with
        coefficient absolute value below the specified threshold
        will be disabled. Disabling useless terms improve
        performance. Default threshold is 10^-10. */
    void set_coefficients_threshold(double t);

    /** This functions must be used to enable/disable a zernike
        term without changing its coefficient */
    void set_term_state(unsigned int n, bool enabled);

    /** Get current term enable state */
    bool get_term_state(unsigned int n);

    /** Compute all zernike coefficient to best fit the given
        curve. RMS difference is returned. The specified
        distribution is used to choose sampling points on
        curve. Terms state is adjusted according to current
        threshold */
    double fit(const TCurveBase &c, const TDistribution & d = default_dist);

    double sagitta(const Math::Vector2 & xy) const;
    void derivative(const Math::Vector2 & xy, Math::Vector2 & dxdy) const;

    /** Evaluate zernike polynomial n */
    static double zernike_poly(unsigned int n, const Math::Vector2 & xy);
    /** Evaluate x and y derivatives of zernike polynomial n */
    static void zernike_poly_d(unsigned int n, const Math::Vector2 & xy, Math::Vector2 & dxdy);
private:

    void update_threshold_state();

    double _scale;
    double _threshold;
    double _radius;
    double _coeff[term_count];
    unsigned int _enabled_count;
    unsigned char _enabled_list[term_count];
};

#endif // TZERNIKE_H
