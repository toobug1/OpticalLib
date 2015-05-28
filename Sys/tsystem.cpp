#include "tsystem.h"
#include "tsurface.h"
#include "Trace/ttraceray.h"

// TODO

Math::Transform<3> * & TSystem::transform_cache_entry(unsigned int from, unsigned int to) const
{
    return const_cast<Math::Transform<3> * &>(_transform_cache[from * _e_count + to]);
}

const Math::Transform<3> & TSystem::get_global_transform(const TElement &from) const
{
    Math::Transform<3> * &e = transform_cache_entry(from.id(), 0);

    // TODO
//    if (!e)
//        transform_l2g_cache_update(from);

    return *e;
}

const Math::Transform<3> & TSystem::transform_l2g_cache_update(const TElement &element) const
{
  Math::Transform<3> * & e = transform_cache_entry(element.id(), 0);

  // TODO
//  if (!e)
//    {
//      Math::Transform<3> t(element._transform);
//      const TElement *i1 = &element;

//      while (const TElement *i2 =
//                             (dynamic_cast<TGroup *>(i1->_container)))
//        {
//          t.compose(i2->_transform);

//          i1 = i2;
//        }

//      assert(!e);
//      e = new Math::Transform<3>(t);
//    }

  return *e;
}

const Math::Transform<3> & TSystem::transform_g2l_cache_update(const TElement &element) const
{
  Math::Transform<3> * & e = transform_cache_entry(0, element.id());

  if (!e)
    e = new Math::Transform<3>(transform_l2g_cache_update(element).inverse());

  return *e;
}
