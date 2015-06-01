#ifndef TPOINTIMAGE_H
#define TPOINTIMAGE_H

#include "common.hh"
#include "Trace/ttracer.h"
#include "Trace/ttraceray.h"
#include "Sys/timage.h"
#include "Sys/tsurface.h"

using namespace _Goptical;


class TSystem;
class TImage;

class TPointImage
{
public:
    TPointImage(TSystem &system);
    virtual ~TPointImage();

    /** set Image which collect rays for analysis */
    void set_image(TImage *image);

    /** return tracer object used for ray tracing. This will
      invalidate current analysis data */
    TTracer & get_tracer();

    /** return tracer object used for ray tracing */
    const TTracer & get_tracer() const;

    /** invalidate current analysis data */
    virtual void invalidate() = 0;

protected:
    void get_default_image();
    void trace();

    TSystem &    _system;
    TTracer      _tracer;
    bool         _processed_trace;
    TImage /*TSurface*/*     _image;
    const rays_queue_t *_intercepts;
};

#endif // TPOINTIMAGE_H
