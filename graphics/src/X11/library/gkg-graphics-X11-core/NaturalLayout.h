#ifndef _gkg_graphics_X11_core_NaturalLayout_h_
#define _gkg_graphics_X11_core_NaturalLayout_h_


#include <gkg-graphics-X11-core/Layout.h>


namespace gkg
{


class NaturalLayout : public Layout
{

  public:

    NaturalLayout( const DimensionName dimensionName, FloatCoordinate natural );
    virtual ~NaturalLayout();

    virtual void request( Glyph::Index count,
                          const GeometryRequisition* geometryRequisitions,
                          GeometryRequisition& result );
    virtual void allocate( const GeometryAllocation& given,
                           Glyph::Index count,
                           const GeometryRequisition* geometryRequisitions,
	                   GeometryAllocation* result );

  private:

    DimensionName _dimensionName;
    FloatCoordinate _natural;

};


}


#endif