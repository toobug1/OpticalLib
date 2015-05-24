#ifndef TFOUCAULT_H
#define TFOUCAULT_H

#include <vector>

#include <gsl/gsl_odeiv.h>

#include "trotational.h"
#include "tcurveroc.h"
#include "Data/tdiscreteset.h"

/*
   @short Define surface curve from Foucault test data.
   @header Goptical/Curve/Foucault
   @module {Core}
   @main

   This class allows definition of a rotationally symmetric curve
   from a set of Foucault test data. Fixed and moving light
   sources are supported.

   Curve fitting can be used to find Foucault test readings from
   any other rotationally symmetric curve.

   Foucault test data are reduced using numerical integration of a
   differential equation to compute sagitta from slopes.

   Foucault data reduction algorithm based on "ATM Mirror Mathematics"
   @url http://home.earthlink.net/~burrjaw/atm/atm_math.lwp/atm_math.htm

   Couder mask theory and formulas developed by Nils Olof Carlin
   @url http://www.atmsite.org/contrib/Carlin/couder/
*/

class TFoucault: public TRotational, public TCurveRoc
{
public:
    /** Create an empty foucault curve with no reading with the
      given radius of curvature */
    TFoucault(double roc);

    ~TFoucault();

    /** Set surface radius. Effective surface radius must be known
      to let the surface integration algorithm run up to a given
      limit. Sagitta and gradient values above the define radius
      won't be accurate.

      Radius is adjusted to max zone radius + 10% when adding
      readings.
  */
    void set_radius(double radius);

    /** Get surface radius. */
    double get_radius() const;

    /** Define a new zone at given zone radius and update knife edge
      reading (offset from radius of curvature) */
    void add_reading(double zone_radius, double knife_offset = 0.0);

    /** Clear current zones and add equally spaced zones
      readings. Knife edge reading (offset from radius of
      curvature) is set to 0 for all zones.
      @return New zones count
  */
    unsigned int add_uniform_zones(double hole_radius, unsigned int count);

    /** Clear current zones and add zones readings to locations
      suited for testing with a couder mask (as described on Nils
      Olof Carlin page). Zone count may be guessed if zero,
      provided that radius has been properly set before. Knife
      edge reading (offset from radius of curvature) is set to 0
      for all zones.
      @param edge Couder zones edges will be saved here if != NULL.
      @return Zone count
  */
    unsigned int add_couder_zones(double hole_radius, unsigned int count = 0, std::vector<double> *edge = 0);

    /** Get currently defined zones/readings count */
    unsigned int get_zones_count() const;

    /** Set knife edge reading of a previously defined zone */
    void set_knife_offset(unsigned int zone_number, double  knife_offset);

    /** Get reading data for a given zone number.
      @return std::pair with zone radius and knife offset
  */
    const std::pair<double, double> get_reading(unsigned int zone_number) const;

    /** Switch to moving source test and update knife edge readings
      of previously defined zone to fit provided rotationally
      symmetric curve. This means simulating a foucault test of
      the given curve using existing zones. */
    void fit(const TRotational &c);

    /** Clear all readings and zones */
    void clear();

    /** Set moving light source foucault test. This clear all
      readings and zones.
      @param source_offset Distance between knife edge and light source along optical axis. */
    void set_moving_source(double source_offset = 0.0);

    /** Set fixed light source foucault test. This clear all
      readings and zones.
      @param source_to_surface Distance between surface and light source along the optical axis. */
    void set_fixed_source(double source_to_surface);

    /** Set surface integration (ODE) algorithm step size, default is 1mm */
    void set_ode_stepsize(double step);

    double sagitta(double r) const;
    double derivative(double r) const;

private:

    void update();
    void init();

    static int gsl_func(double t, const double y[], double f[], void *params);
    gsl_odeiv_step *gsl_st;
    gsl_odeiv_system gsl_sys;

    bool _moving_source;
    double _offset;
    double _radius;
    double _ode_step;
    TDiscreteSet _reading;
    TDiscreteSet _sagitta;
    bool _updated;
};

#endif // TFOUCAULT_H
