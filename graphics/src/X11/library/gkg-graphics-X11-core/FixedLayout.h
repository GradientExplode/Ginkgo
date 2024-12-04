#ifndef _gkg_graphics_X11_core_FixedLayout_h_
#define _gkg_graphics_X11_core_FixedLayout_h_


#include <gkg-graphics-X11-core/Layout.h>


namespace gkg
{


class FixedLayout : public Layout
{

  public:

    FixedLayout( const DimensionName dimensionName, FloatCoordinate span );
    virtual ~FixedLayout();

    virtual void request( Glyph::Index count,
                          const GeometryRequisition* geometryRequisitions,
                          GeometryRequisition& result );
    virtual void allocate( const GeometryAllocation& given,
                           Glyph::Index count,
                           const GeometryRequisition* geometryRequisitions,
	                   GeometryAllocation* result );

  private:

    DimensionName _dimensionName;
    FloatCoordinate _span;

};


}


#endif
