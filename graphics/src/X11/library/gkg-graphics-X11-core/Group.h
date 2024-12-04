#ifndef _gkg_graphics_X11_core_Group_h_
#define _gkg_graphics_X11_core_Group_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Aggregate;
class Layout;


class Group : public Glyph
{

  public:

    Group( Aggregate* aggregate, DimensionName dimensionName );
    virtual ~Group();

    void map( Glyph::Index index );

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );

  private:

    Aggregate* _aggregate;
    DimensionName _dimensionName;
    Layout* _layout;
    Glyph::Index* _map;
    int32_t _count;

};


}


#endif
