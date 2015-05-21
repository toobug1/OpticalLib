#include "tcomposer.h"

TComposer::TAttributes::TAttributes(TCurveBase& base)
{
    QSharedPointer<TCurveBase> ptr(&base);
    _curve = ptr;
    _z_scale = 1.;
    _z_offset = 0.;
    _transform.reset();
    _inv_transform.reset();
}



TComposer::TAttributes & TComposer::TAttributes::z_scale(double zfactor)
{
  _z_scale *= zfactor;

  return *this;
}

TComposer::TAttributes & TComposer::TAttributes::z_offset(double zoffset)
{
  _z_offset += zoffset;

  return *this;
}

TComposer::TAttributes & TComposer::TAttributes::xy_scale(const Math::Vector2 &factor)
{
  _transform.affine_scaling(factor);
  _inv_transform = _transform.inverse();

  return *this;
}

TComposer::TAttributes & TComposer::TAttributes::rotate(double angle)
{
  _transform.affine_rotation(0, angle);
  _inv_transform = _transform.inverse();

  return *this;
}

TComposer::TAttributes & TComposer::TAttributes::xy_translate(const Math::Vector2 &offset)
{
  _transform.apply_translation(offset);
  _inv_transform = _transform.inverse();

  return *this;
}

TComposer::TAttributes & TComposer::add_curve(TCurveBase& curve)
{
  TAttributes attr(curve);

  _list.push_back(attr);

  return _list.back();
}

TComposer::TComposer()
  : _list()
{
}

double TComposer::sagitta(const Math::Vector2 & xy) const
{
  double z = 0;


  GOPTICAL_FOREACH(c, _list)
    z += c->_curve->sagitta(c->_inv_transform.transform(xy)) * c->_z_scale + c->_z_offset;

  return z;
}

void TComposer::derivative(const Math::Vector2 & xy, Math::Vector2 & dxdy) const
{
  dxdy.set(0.0);

  GOPTICAL_FOREACH(c, _list)
    {
      Math::Vector2 dtmp;

      c->_curve->derivative(c->_inv_transform.transform(xy), dtmp);

      dxdy += c->_transform.transform_linear(dtmp) * c->_z_scale;
    }
}
