#include "tcurvebase.h"

#include <gsl/gsl_deriv.h>

// Default curve derivative use gsl numerical differentiation

struct curve_gsl_params_s
{
  const TCurveBase *c;
  double x, y;
};


TCurveBase::TCurveBase()
{

}

TCurveBase::~TCurveBase()
{

}

static double gsl_func_sagitta_x(double x, void *params)
{
  struct curve_gsl_params_s *p = (struct curve_gsl_params_s*)params;

  return p->c->sagitta(TVector2D(x, p->y));
}

static double gsl_func_sagitta_y(double y, void *params)
{
  struct curve_gsl_params_s *p = (struct curve_gsl_params_s*)params;

  return p->c->sagitta(TVector2D(p->x, y));
}

void TCurveBase::derivative(const TVector2D & xy, TVector2D & dxdy) const
{
    double abserr;
    struct curve_gsl_params_s params;
    gsl_function gsl_func;

    gsl_func.params = &params;

    params.c = this;
    params.x = xy.x();
    params.y = xy.y();

    gsl_func.function = gsl_func_sagitta_x;
    double resultX, resultY;
    gsl_deriv_central(&gsl_func, xy.x(), 1e-6, &resultX, &abserr);

    gsl_func.function = gsl_func_sagitta_y;
    gsl_deriv_central(&gsl_func, xy.y(), 1e-6, &resultY, &abserr);

    dxdy.setX(resultX);
    dxdy.setY(resultY);
}


 bool TCurveBase::intersect(TVector3D &point, const TVectorPair3D &ray) const
 {
     TVectorPair3D p;

     // initial intersection with z=0 plane
     {
       double  s = ray.direction().z();

       if (s == 0)
         return false;

       double  a = -ray.origin().z() / s;

       if (a < 0)
         return false;

       p.origin() = ray.origin() + ray.direction() * a;
     }

     unsigned int n = 32;      // avoid infinite loop

     while (n--)
       {
         double new_sag = sagitta(p.origin().project_xy());
         double old_sag = p.origin().z();

         // project previous intersection point on curve
//         p.origin().z() = new_sag;
         p.origin().setZ(new_sag);

         // stop if close enough
         if (fabs(old_sag - new_sag) < 1e-10)
           break;

         // get curve tangeante plane at intersection point
         normal(p.normal(), p.origin());

         // intersect again with new tangeante plane
         double a = p.pl_ln_intersect_scale(ray);

         if (a < 0)
           return false;

         p.origin() = ray.origin() + ray.direction() * a;
       }

     point = p.origin();

     return true;
 }

