#ifndef _gkg_graphics_X11_core_CenterLayout_h_
#define _gkg_graphics_X11_core_CenterLayout_h_


#include <gkg-graphics-X11-core/Layout.h>


namespace gkg
{


class CenterLayout : public Layout
{

  public:

    CenterLayout( const DimensionName dimensionName, float alignment = 0.5 );
    virtual ~CenterLayout();

    virtual void request( Glyph::Index count,
                          const GeometryRequisition* geometryRequisitions,
                          GeometryRequisition& result );
    virtual void allocate( const GeometryAllocation& given,
                           Glyph::Index count,
                           const GeometryRequisition* geometryRequisitions,
	                   GeometryAllocation* result );

  private:

    DimensionName _dimensionName;
    float _alignment;

};


}


#endif
