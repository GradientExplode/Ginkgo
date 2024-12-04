#ifndef _gkg_graphics_X11_core_Layout_h_
#define _gkg_graphics_X11_core_Layout_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


// structured layout
class Layout
{

  public:

    virtual ~Layout();

    virtual void request( Glyph::Index count,
                          const GeometryRequisition* geometryRequisitions,
                          GeometryRequisition& result );
    virtual void allocate( const GeometryAllocation& given,
                           Glyph::Index count,
                           const GeometryRequisition* geometryRequisitions,
	                   GeometryAllocation* result );

  protected:

    Layout();

};


}


#endif
