#ifndef TFOCUS_H
#define TFOCUS_H

#include "tpointimage.h"
#include "Math/VectorPair"

class TFocus : public TPointImage
{
public:
    TFocus(Sys::TSystem &system);

    inline void invalidate();

    /** Get best point of focus in system global coordinates. */
    inline const Math::VectorPair3 & get_best_focus();

private:
    void process_focus();

    bool              _processed_focus;
    Math::VectorPair3 _best_focus;
};

#endif // TFOCUS_H
