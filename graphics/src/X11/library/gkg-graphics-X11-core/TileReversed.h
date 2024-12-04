#ifndef _gkg_graphics_X11_core_TileReversed_h_
#define _gkg_graphics_X11_core_TileReversed_h_


#include <gkg-graphics-X11-core/Layout.h>


namespace gkg
{


// tiled layout
class TileReversed : public Layout
{

  public:

    TileReversed( DimensionName dimensionName );
    virtual ~TileReversed();

    virtual void request( Glyph::Index count,
                          const GeometryRequisition* geometryRequisitions,
                          GeometryRequisition& result );
    virtual void allocate( const GeometryAllocation& given,
                           Glyph::Index count,
                           const GeometryRequisition* geometryRequisitions,
	                   GeometryAllocation* result );

  private:

    DimensionName _dimensionName;
    GeometryRequisition _geometryRequisition;

};


}


#endif
