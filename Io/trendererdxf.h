#ifndef TRENDERERDXF_H
#define TRENDERERDXF_H

#include <dime/Model.h>
#include <dime/sections/EntitiesSection.h>
#include <dime/sections/TablesSection.h>
#include <dime/sections/BlocksSection.h>
#include <dime/tables/LayerTable.h>
#include <dime/tables/Table.h>
#include <dime/entities/Block.h>
#include <dime/entities/Insert.h>
#include <dime/util/Linear.h>

#include "trenderer.h"

class TRendererDxf : public TRenderer
{
public:
    /** Create a DXF renderer */
    TRendererDxf();

    /** Create a DXF renderer, output will be written to given
      filename when the renderer is destroyed. */
    TRendererDxf(const char *filename);

    ~TRendererDxf();

    /** Add a new layer in dxf file and return its index */
    unsigned int add_layer(std::string name);
    /** Set current layer */
    void use_layer(unsigned int id);

    /** Save output to a DXF file */
    void write(const std::string & filename);

private:
    void init();

    /** @override */
    void draw_point(const Math::Vector2 &p,
                    const TRgb &rgb,
                    enum PointStyle s);
    /** @override */
    void draw_segment(const Math::VectorPair2 &l, const TRgb &rgb);
    /** @override */
    void draw_circle(const Math::Vector2 &c,
                     double r,
                     const TRgb &rgb,
                     bool filled);
    /** @override */
    void draw_text(const Math::Vector2 &pos,
                   const Math::Vector2 &dir,
                   const std::string &str,
                   TextAlignMask a,
                   int size,
                   const TRgb &rgb);
    /** @override */
    void draw_text(const Math::Vector3 &pos,
                   const Math::Vector3 &dir,
                   const std::string &str,
                   TextAlignMask a,
                   int size,
                   const TRgb &rgb);
    /** @override */
    void draw_point(const Math::Vector3 &p,
                    const TRgb &rgb,
                    enum PointStyle s);
    /** @override */
    void draw_segment(const Math::VectorPair3 &l, const TRgb &rgb);

    /** @override */
    void group_begin(const std::string &name);
    /** @override */
    void group_end();

    void add_entity(dimeEntity *entity);

    const char *_filename;
    dimeLayerTable *current_layer();

    dimeVec3f vec3fconv(const Math::Vector3 &v);
    dimeVec3f vec3fconv(const Math::Vector2 &v);

    unsigned int _group_level;

    dimeModel _model;
    dimeTablesSection *_table_section;
    dimeTable *_layers_table;
    dimeEntitiesSection *_entities_section;
    dimeBlocksSection *_blocks_section;

    dimeBlock *_current_block;
    unsigned int _layer_id;
};

#endif // TRENDERERDXF_H
