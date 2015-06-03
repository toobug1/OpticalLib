#include "trendererdxf.h"

#include <dime/entities/Point.h>
#include <dime/entities/Circle.h>
#include <dime/entities/Line.h>
#include <dime/Output.h>

void TRendererDxf::init()
{
    // define layers
    _table_section = new dimeTablesSection();
    _model.insertSection(_table_section);

    _layers_table = new dimeTable(0);
    _table_section->insertTable(_layers_table);

    _blocks_section = new dimeBlocksSection();
    _model.insertSection(_blocks_section);

    _entities_section = new dimeEntitiesSection();
    _model.insertSection(_entities_section);

    //      add_layer("0");
}

TRendererDxf::TRendererDxf(const char *filename)
    : _filename(filename),
      _group_level(0),
      _model(),
      _current_block(0),
      _layer_id(0)
{
    init();
}

TRendererDxf::TRendererDxf()
    : _filename(0),
      _group_level(0),
      _model(),
      _current_block(0),
      _layer_id(0)
{
    init();
}

TRendererDxf::~TRendererDxf()
{
    if (_filename)
        write(_filename);

#if 1 // LQ FIX 0 to 1
    // FIXME figure out which dime objects must be deleted
    delete _entities_section;
    delete _blocks_section;
    delete _layers_table;
    delete _table_section;
#endif
}

unsigned int TRendererDxf::add_layer(std::string name)
{
    dimeLayerTable *layer = new dimeLayerTable;

    layer->setLayerName(name.c_str(), 0);
    layer->setColorNumber(0);

    // need to set some extra records so that AutoCAD will stop
    // complaining (see dime library sphere example)
    dimeParam param;
    param.string_data = "CONTINUOUS";
    layer->setRecord(6, param);
    param.int16_data = 64;
    layer->setRecord(70, param);

    layer->registerLayer(&_model);

    return _model.getNumLayers() - 1;
}

void TRendererDxf::add_entity(dimeEntity *entity)
{
    entity->setLayer(_model.getLayer(_layer_id));

    if (_current_block)
        _current_block->insertEntity(entity);
    else
        _model.addEntity(entity);
}

void TRendererDxf::write(const std::string & filename)
{
    dimeOutput out;

    out.setFilename(filename.c_str());
    _model.write(&out);

    _filename = 0;
}

void TRendererDxf::group_begin(const std::string &name)
{
    if (_group_level++)
        return;

    _current_block = new dimeBlock(0);
    _current_block->setName(name.c_str());
    _blocks_section->insertBlock(_current_block);
}

void TRendererDxf::group_end()
{
    if (--_group_level)
        return;

    dimeInsert *insert = new dimeInsert();
    insert->setBlock(_current_block);
    _model.addEntity(insert);

    _current_block = 0;
}

void TRendererDxf::draw_point(const Math::Vector2 &p,
                              const TRgb &rgb,
                              enum PointStyle s)
{
    dimePoint *point = new dimePoint;

    point->setCoords(vec3fconv(p));
    add_entity(point);
}

void TRendererDxf::draw_segment(const Math::VectorPair2 &l,
                                const TRgb &rgb)
{
    dimeLine *line = new dimeLine;

    line->setCoords(0, vec3fconv(l[0]));
    line->setCoords(1, vec3fconv(l[1]));
    add_entity(line);
}

#if 0
void TRendererDxf::draw_polygon(const Math::Vector2 *array,
                                unsigned int count,
                                const TRgb &rgb,
                                bool filled,
                                bool closed)
{
    if (count < 3)
        return;

    dimePolyline *pline = new dimePolyline;
    dimeVertex *vtxct[count];

    for (unsigned int i = 0; i < count; i++)
    {
        dimeVertex *v = new dimeVertex;
        v->setCoords(vec3fconv(array[i]));
        v->setFlags(dimeVertex::POLYLINE_3D_VERTEX);
        vtxct[i] = v;
    }

    pline->setSurfaceType(dimePolyline::POLYLINE);
    pline->setFlags(dimePolyline::IS_POLYLINE_3D);
    pline->setCoordVertices(vtxct, count);

    for (unsigned int i = 0; i < count - 1; i++)
    {
        dimeVertex *v = new dimeVertex;
        v->setIndex(0, i);
        v->setIndex(1, i+1);
        v->setFlags(dimeVertex::POLYLINE_3D_VERTEX);
        vtxct[i] = v;
    }

    pline->setIndexVertices(vtxct, count - 1);

    add_entity(pline);
}
#endif

void TRendererDxf::draw_circle(const Math::Vector2 &c,
                               double r,
                               const TRgb &rgb,
                               bool filled)
{
    dimeCircle *circle = new dimeCircle;

    circle->setCenter(vec3fconv(c));
    circle->setRadius(r);

    add_entity(circle);
}

void TRendererDxf::draw_text(const Math::Vector2 &pos,
                             const Math::Vector2 &dir,
                             const std::string &str,
                             TextAlignMask a,
                             int size,
                             const TRgb &rgb)
{
    // FIXME
}

void TRendererDxf::draw_text(const Math::Vector3 &pos,
                             const Math::Vector3 &dir,
                             const std::string &str,
                             TextAlignMask a,
                             int size,
                             const TRgb &rgb)
{
    // FIXME
}

void TRendererDxf::draw_point(const Math::Vector3 &p,
                              const TRgb &rgb,
                              enum PointStyle s)
{
    dimePoint *point = new dimePoint;

    point->setCoords(vec3fconv(p));
    add_entity(point);
}

void TRendererDxf::draw_segment(const Math::VectorPair3 &l, const TRgb &rgb)
{
    dimeLine *line = new dimeLine;

    line->setCoords(0, vec3fconv(l[0]));
    line->setCoords(1, vec3fconv(l[1]));

    add_entity(line);
}

dimeVec3f TRendererDxf::vec3fconv(const Math::Vector3 &v)
{
    return dimeVec3f(v.x(), v.y(), v.z());
}

dimeVec3f TRendererDxf::vec3fconv(const Math::Vector2 &v)
{
    return dimeVec3f(v.x(), v.y(), 0.0);
}

void TRendererDxf::use_layer(unsigned int id)
{
    assert(id < (unsigned)_model.getNumLayers());
    _layer_id = id;
}

