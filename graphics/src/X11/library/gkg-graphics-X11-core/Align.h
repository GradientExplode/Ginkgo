#ifndef _gkg_graphics_X11_core_Align_h_
#define _gkg_graphics_X11_core_Align_h_


#include <gkg-graphics-X11-core/Layout.h>


namespace gkg
{


// aligned layout
class Align : public Layout
{

  public:

    Align( DimensionName dimensionName );
    virtual ~Align();

    virtual void request( Glyph::Index count,
                          const GeometryRequisition* geometryRequisitions,
                          GeometryRequisition& result );
    virtual void allocate( const GeometryAllocation& given,
                           Glyph::Index count,
                           const GeometryRequisition* geometryRequisitions,
	                   GeometryAllocation* result );

  private:

    DimensionName _dimensionName;

};


}


#endif
