#ifndef TTCATALOG_H
#define TTCATALOG_H

#include <string>
#include <QSharedPointer>
#include <map>

#include "tglass.h"

class TCatalog
{
public:
    /** Create a catalog with given name */
    TCatalog(const std::string & name = "");

    /** Get catalog name */
    const std::string & get_name() const;
    /** Set catalog name */
    void set_name(const std::string & name);

    /** Get material with given name */
    const TGlass & get_material(const std::string &material_name);

    /** Add a material to catalog. material object will be deleted
      on catalog destruction if owner is set. */
    void add_material(const std::string &material_name,
                      const QSharedPointer<TGlass> &material);

    /** Remove a material from catalog */
    void del_material(const std::string &material_name);

private:

    typedef std::map<std::string, QSharedPointer<TGlass> > catalog_map_t;

    std::string _name;
    catalog_map_t _list;
};

#endif // TTCATALOG_H
