#ifndef TPOINTIMAGE_H
#define TPOINTIMAGE_H

#include "common.hh"
#include "Trace/ttracer.h"

using namespace _Goptical;

class TPointImage
{
public:
    TPointImage(Sys::TSystem &system);
    virtual ~TPointImage();

    /** set Image which collect rays for analysis */
    inline void set_image(Sys::Image *image);

    /** return tracer object used for ray tracing. This will
      invalidate current analysis data */
    inline TTracer & get_tracer();

    /** return tracer object used for ray tracing */
    inline const TTracer & get_tracer() const;

    /** invalidate current analysis data */
    virtual void invalidate() = 0;

protected:
    void get_default_image();
    void trace();

    Sys::TSystem &    _system;
    TTracer           _tracer;
    bool              _processed_trace;
    Sys::Image *      _image;
    const Trace::rays_queue_t *_intercepts;
};

#endif // TPOINTIMAGE_H
