#ifndef _gkg_graphics_X11_core_VariableLayout_h_
#define _gkg_graphics_X11_core_VariableLayout_h_


#include <gkg-graphics-X11-core/Layout.h>


namespace gkg
{


class VariableLayout : public Layout
{

  public:

    VariableLayout( const DimensionName dimensionName,
                    FloatCoordinate stretch,
                    FloatCoordinate shrink );
    virtual ~VariableLayout();

    virtual void request( Glyph::Index count,
                          const GeometryRequisition* geometryRequisitions,
                          GeometryRequisition& result );
    virtual void allocate( const GeometryAllocation& given,
                           Glyph::Index count,
                           const GeometryRequisition* geometryRequisitions,
	                   GeometryAllocation* result );

  private:

    DimensionName _dimensionName;
    FloatCoordinate _stretch;
    FloatCoordinate _shrink;

};


}


#endif
