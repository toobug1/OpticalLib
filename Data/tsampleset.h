#ifndef TSAMPLESET_H
#define TSAMPLESET_H

#include "tsamplesetbase.h"
#include "tinterpolate1d.h"

class TSampleSet : public TInterpolate1d<TSampleSetBase>
{
public:
    TSampleSet(): TInterpolate1d<TSampleSetBase>()
    {}

};

#endif // TSAMPLESET_H
