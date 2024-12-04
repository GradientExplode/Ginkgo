#ifndef _gkg_graphics_X11_core_Rule_h_
#define _gkg_graphics_X11_core_Rule_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Color;


// visible filling
class Rule : public Glyph
{

  public:

    Rule( DimensionName dimensionName,
          const Color* color,
          FloatCoordinate thickness );
    virtual ~Rule();

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;

  private:

    DimensionName _dimensionName;
    const Color* _color;
    FloatCoordinate _thickness;
};


}


#endif
