#include "tflat.h"

TFlat::TFlat()
{
}

double TFlat::sagitta(double r) const
{
  return 0;
}

double TFlat::derivative(double r) const
{
  return 1.0;
}

/*

intersection d'un plan defini par :

P(Px, Py, Pz) appartenant au plan
N(Px, Py, Pz) normal au plan

avec une droite AB definie par l'ensemble des points tel que:

A + * t B

on a :

t=(Nz*Pz+Ny*Py+Nx*Px-Az*Nz-Ay*Ny-Ax*Nx)/(Bz*Nz+By*Ny+Bx*Nx)

*/

bool TFlat::intersect(Math::Vector3 &point, const Math::VectorPair3 &ray) const
{
  double    s = ray.direction().z();

  if (s == 0)
    return false;

  double    a = -ray.origin().z() / s;

  if (a < 0)
    return false;

  point = ray.origin() + ray.direction() * a;

  return true;
}

void TFlat::normal(Math::Vector3 &normal, const Math::Vector3 &point) const
{
  normal = Math::Vector3(0, 0, -1);
}

TFlat flat;

