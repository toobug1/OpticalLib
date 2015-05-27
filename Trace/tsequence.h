#ifndef TSEQUENCE_H
#define TSEQUENCE_H

#include <QSharedPointer>

#include "Sys/tsystem.h"
#include "Sys/telement.h"
#include "Sys/tcontainer.h"

class TSequence
{
    friend std::ostream & operator<<(std::ostream &o, const TSequence &s);
    friend class Tracer;

public:
    /** Create a new empty sequence */
    TSequence();

    /** Create a new sequence and insert all elements present in the
        system. This is equivalent to calling add() on empty sequence. */
    TSequence(const TSystem &system);

    /** Add all elements from the given system. Element are sorted
        in axis order starting from left; reflecting elements do reverse
        direction. */
    void add(const TSystem &system);

    /** Insert an element at end of sequence.
        @return position of the element in the sequence
    */
    unsigned int append(const TElement &element);

    /** Insert an element in sequence at given position */
    void insert(unsigned int index, const TElement &element);

    /** Remove an element from sequence */
    void remove(unsigned int index);

    /** Remove all elements from sequence */
    void clear();

    /** Get a reference to an element in sequence */
    const TElement &get_element(unsigned int index) const;

private:
    void add(const TContainer &c);

    std::vector<QSharedPointer<const TElement> > _list;
};

#endif // TSEQUENCE_H
