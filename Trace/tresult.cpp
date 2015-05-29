#include "tresult.h"
#include "Sys/telement.h"
#include "Io/trenderer.h"

TResult::element_result_s & TResult::get_element_result(const TElement &e)
{
  return _elements[e.id() - 1];
}

const TResult::element_result_s & TResult::get_element_result(const TElement &e) const
{
  return _elements[e.id() - 1];
}

const rays_queue_t & TResult::get_intercepted(const TSurface &s) const
{
  const struct element_result_s &er = get_element_result(s);

  if (!er._intercepted)
    throw Error("no such ray interception surface in ray trace TResult");

  return *er._intercepted;
}

const rays_queue_t & TResult::get_generated(const TElement &e) const
{
  const struct element_result_s &er = get_element_result(e);

  if (!er._generated)
    throw Error("no such ray generator element in ray trace TResult");

  return *er._generated;
}

const TResult::sources_t & TResult::get_source_list() const
{
  return _sources;
}

void TResult::add_intercepted(const TSurface &s, TTraceRay &ray)
{
  element_result_s &er = get_element_result(s);

  if (er._intercepted)
    er._intercepted->push_back(&ray);
}

void TResult::add_generated(const TElement &s, TTraceRay &ray)
{
  element_result_s &er = get_element_result(s);

  if (er._generated)
    er._generated->push_back(&ray);
}

void TResult::add_ray_wavelen(double wavelen)
{
  _wavelengths.insert(wavelen);
}

const std::set<double> & TResult::get_ray_wavelen_set() const
{
  return _wavelengths;
}

TTraceRay & TResult::new_ray()
{
  TTraceRay        &r = _rays.create();

  if (_generated_queue)
    _generated_queue->push_back(&r);

  return r;
}

TTraceRay & TResult::new_ray(const TRay &ray)
{
  TTraceRay        &r = _rays.create(ray);

  if (_generated_queue)
    _generated_queue->push_back(&r);

  return r;
}

const TParams & TResult::get_params() const
{
  assert(_params != 0);
  return *_params;
}

TResult::TResult()
  : _rays(),
    _elements(),
    _wavelengths(),
    _generated_queue(0),
    _sources(),
    _bounce_limit_count(0),
    _system(0)
{
}

TResult::~TResult()
{
  clear();
}

void TResult::clear_save_states()
{
  GOPTICAL_FOREACH(i, _elements)
    {
      i->_save_intercepted_list = false;
      i->_save_generated_list = false;
    }
}

void TResult::clear()
{
  GOPTICAL_FOREACH(i, _elements)
    {
      if (i->_intercepted)
        {
          delete i->_intercepted;
          i->_intercepted = 0;
        }

      if (i->_generated)
        {
          delete i->_generated;
          i->_generated = 0;
        }
    }

  _rays.clear();
  _sources.clear();
  _wavelengths.clear();

  _bounce_limit_count = 0;
}

void TResult::prepare()
{
  clear();

  GOPTICAL_FOREACH(i, _elements)
    {
      if (i->_save_intercepted_list)
        i->_intercepted = new rays_queue_t;

      if (i->_save_generated_list)
        i->_generated = new rays_queue_t;
    }
}

void TResult::init(const TSystem &system)
{
  static const struct element_result_s er = {0, 0, false, false};

  if (!_system)
    _system = &system;

  if(_system != &system)
    throw Error("Trace::TResult used with multiple Sys::System objects");

  _elements.resize(system.get_element_count(), er);
}

void TResult::init(const TElement &element)
{
  const TSystem *system = element.get_system();

  assert(system != 0);

  init(*system);
}

void TResult::set_intercepted_save_state(const TElement &e, bool enabled)
{
  init(e);
  get_element_result(e)._save_intercepted_list = enabled;
}

void TResult::set_generated_save_state(const TElement &e, bool enabled)
{
  init(e);
  get_element_result(e)._save_generated_list = enabled;
}

bool TResult::get_intercepted_save_state(const TElement &e)
{
  return get_element_result(e)._save_intercepted_list;
}

bool TResult::get_generated_save_state(const TElement &e)
{
  return get_element_result(e)._save_generated_list;
}


Math::VectorPair3 TResult::get_intercepted_window(const TSurface &s) const
{
  const rays_queue_t & intercepts = get_intercepted(s);

  if (intercepts.empty())
    throw Error("no ray intercepts found on the surface");

  const Math::Vector3 &first = (*intercepts.begin())->get_intercept_point();
  Math::VectorPair3 window(first, first);

  GOPTICAL_FOREACH(i, intercepts)
    {
      const Math::Vector3 &ip = (*i)->get_intercept_point();

      if (window[0].x() > ip.x())
        window[0].x() = ip.x();
      else if (window[1].x() < ip.x())
        window[1].x() = ip.x();

      if (window[0].y() > ip.y())
        window[0].y() = ip.y();
      else if (window[1].y() < ip.y())
        window[1].y() = ip.y();

      if (window[0].z() > ip.z())
        window[0].z() = ip.z();
      else if (window[1].z() < ip.z())
        window[1].z() = ip.z();
    }

  return window;
}

Math::Vector3 TResult::get_intercepted_center(const TSurface &s) const
{
  Math::VectorPair3 win = get_intercepted_window(s);

  return (win[0] + win[1]) / 2;
}

Math::Vector3 TResult::get_intercepted_centroid(const TSurface &s) const
{
  const rays_queue_t & intercepts = get_intercepted(s);
  unsigned int      count = 0;
  Math::Vector3     center(0, 0, 0);

  if (intercepts.empty())
    throw Error("no ray intercepts found on the surface");

  GOPTICAL_FOREACH(i, intercepts)
    {
      center += (*i)->get_intercept_point();
      count++;
    }

  center /= count;

  return center;
}

double TResult::get_max_ray_intensity() const
{
  double res = 0;

  GOPTICAL_FOREACH(r, _rays)
    {
      double i = r->get_intensity();

      if (i > res)
        res = i;
    }

  return res;
}

void TResult::draw_2d(TRenderer &r, bool hit_image, const TElement *ref) const
{
  r.draw_trace_result_2d(*this, hit_image, ref);
}

void TResult::draw_3d(TRenderer &r, bool hit_image, const TElement *ref) const
{
  r.draw_trace_result_3d(*this, hit_image, ref);
}
