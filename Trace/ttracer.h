#ifndef TTRACER_H
#define TTRACER_H

#include <QSharedPointer>

#include "Sys/tsystem.h"
#include "tresult.h"
#include "tparams.h"

class TTracer
{
public:

    /** Create a new Light porpagator object */
    TTracer(QSharedPointer<TSystem> system);

    ~TTracer();

    /** Set the Result object which must be used to store ray
      tracing data. a new Result object will be allocated on
      first ray trace operation if none were defined. */
    void set_trace_result(TResult &res);

    /** replace all tracer parameters */
    void set_params(const TParams &params);
    /** get tracer parameters */
    const TParams & get_params() const;
    /** get tracer parameters */
    TParams & get_params();

    /* Get current trace result object */
    TResult & get_trace_result() const;

    /** Undefine user defined Result object. Next ray trace
      operation will allocate a new internal trace result object */
    TResult & set_default_trace_result();

    /** Get attached system */
    const TSystem & get_system() const;

    /** Launch ray tracing operation */
    void trace();

private:

    template <Trace::IntensityMode m> void trace_template();
    template <Trace::IntensityMode m> void trace_seq_template();

    QSharedPointer<TSystem>    _system;
    TParams                    _params;
    TResult                    _result;
    TResult                    *_result_ptr;
};

#endif // TTRACER_H
