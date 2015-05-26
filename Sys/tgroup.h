#ifndef TGROUP_H
#define TGROUP_H

#include "telement.h"
#include "tcontainer.h"
#include "Math/VectorPair"

class TSystem;

class TGroup : public TElement, public TContainer
{
public:

    /** Create a new group at given position */
    inline TGroup(const Math::VectorPair3 &p);

    virtual ~TGroup();

    Math::VectorPair3 get_bounding_box() const;

protected:
    /** @override */
    void draw_2d_e(Io::Renderer &r, const TElement *ref) const;
    /** @override */
    void draw_3d_e(Io::Renderer &r, const TElement *ref) const;

private:
    void added(TElement &e);
    void removed(TElement &e);

    void system_register(TSystem &s);
    void system_unregister();
    void system_moved();
};

#endif // TGROUP_H
