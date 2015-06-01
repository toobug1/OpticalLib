#include "tpointimage.h"
#include "Trace/tresult.h"

void TPointImage::set_image(TImage *image)
{
    _image = image;
}

TTracer & TPointImage::get_tracer()
{
    invalidate();
    return _tracer;
}

/** return tracer object used for ray tracing */
const TTracer & TPointImage::get_tracer() const
{
    return _tracer;
}

TPointImage::TPointImage(TSystem &system)
    : _system(system), _tracer(QSharedPointer<TSystem>(&system)),
      _processed_trace(false),
      _image(0),
      _intercepts(0)
{
    _tracer.get_params().get_default_distribution().set_uniform_pattern();
}

TPointImage::~TPointImage()
{
}

void TPointImage::get_default_image()
{
    if (!_image)
        _image = _system.find<TImage>();

    if (!_image)
        throw Error("no image found for analysis");
}

void TPointImage::trace()
{
    if (_processed_trace)
        return;

    TResult &result = _tracer.get_trace_result();

    get_default_image();
    result.set_intercepted_save_state(*_image, true);
    _tracer.trace();

    //      if (_sys_system.has_exit_pupil())
    //        result.discard_intercepts_not_from(*_image, _system.get_exit_pupil());

    _intercepts = &result.get_intercepted(*_image);

    _processed_trace = true;
}
