#include "tcatalog.h"
#include "Error"

using namespace _Goptical;

TCatalog::TCatalog(const std::string & name)
    : _name(name),
      _list()
{
}

void TCatalog::add_material(const std::string &material_name,
                            const QSharedPointer<TGlass> &material)
{
    if (!_list.insert(catalog_map_t::value_type(material_name, material)).second)
        throw Error("Material already present in TCatalog");
}

void TCatalog::del_material(const std::string &material_name)
{
    _list.erase(material_name);
}

const std::string & TCatalog::get_name() const
{
    return _name;
}

void TCatalog::set_name(const std::string & name)
{
    _name = name;
}

const TGlass & TCatalog::get_material(const std::string &material_name)
{
    catalog_map_t::const_iterator i = _list.find(material_name);


    if (i == _list.end())
        throw Error("No such material in TCatalog");

    return *(i->second);
}

