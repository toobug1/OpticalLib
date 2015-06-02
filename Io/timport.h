#ifndef TIMPORT_H
#define TIMPORT_H

#include "Sys/tsystem.h"
#include "Data/tset.h"
#include "Material/tcatalog.h"

class TImport
{
public:
    /** Import optical design from file */
    virtual TSystem & import_design(const std::string &filename) = 0;

    /** Import material catalog from file */
    virtual TCatalog & import_catalog(const std::string &filename) = 0;

    /** Import dataset from file */
    virtual TSet & import_dataset(const std::string &filename) = 0;
};

#endif // TIMPORT_H
