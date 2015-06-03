#ifndef TIMPORTZEMAX_H
#define TIMPORTZEMAX_H

#include <string>
#include <map>

#include <QSharedPointer>

#include "common.hh"

#include "Shape/tshapebase.h"
#include "Material/tmaterialbase.h"
#include "Material/tcatalog.h"
#include "Material/tdielectricbase.h"
#include "Sys/tsystem.h"

struct zemax_surface_s;

class TImportZemax
{
public:
    /** @experimental */
    QSharedPointer<TSystem> import_design(const std::string &filename);

    /** Set glass catalogs default path */
    void set_catalog_path(const std::string &path);

    /** Import Zemax ascii glass catalog, guess filename from default path and name */
    TCatalog& import_catalog(const std::string &name);

    /** Import Zemax ascii glass catalog file (@tt .agf). Guess catalog
      name from file name */
    TCatalog& import_catalog_file(const std::string &path);

    /** Import Zemax ascii glass catalog file (@tt .agf) */
    TCatalog& import_catalog(const std::string &path,
                             const std::string &name);

    /** Get already imported catalog */
    TCatalog& get_catalog(const std::string &name);

    /** Import Zemax table glass material file (@tt .ztg) */
//    TDielectricBase& import_table_glass(const std::string &filename);

private:

    static std::string basename(const std::string &path);

    QSharedPointer<TShapeBase> get_ap_shape(const struct zemax_surface_s &surf, double unit_factor) const;
    const TGlass& get_glass(TSystem &sys, const struct zemax_surface_s &surf) const;

    typedef std::map<std::string, QSharedPointer<TCatalog> > cat_map_t;

    cat_map_t         _cat_list;
    std::string       _cat_path;
};

#endif // TIMPORTZEMAX_H
