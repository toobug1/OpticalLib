#ifndef TTRACERAY_H
#define TTRACERAY_H

#include <deque>

class TTraceRay
{
public:
    TTraceRay();
    ~TTraceRay();
};

typedef std::deque<TTraceRay *> rays_queue_t;

#endif // TTRACERAY_H
