#ifndef _gkg_graphics_X11_core_Superpose_h_
#define _gkg_graphics_X11_core_Superpose_h_


#include <gkg-graphics-X11-core/Layout.h>


namespace gkg
{


// composite layout
class Superpose : public Layout
{

  public:

    Superpose( Layout* layout0,
               Layout* layout1,
               Layout* layout2 = 0,
               Layout* layout3 = 0,
               Layout* layout4 = 0 );
    virtual ~Superpose();

    virtual void request( Glyph::Index count,
                          const GeometryRequisition* geometryRequisition,
                          GeometryRequisition& result );
    virtual void allocate( const GeometryAllocation& given,
                           Glyph::Index count,
                           const GeometryRequisition* geometryRequisition,
	                   GeometryAllocation* result );

  private:

    Layout** _layouts;
    int32_t _count;

};


}


#endif
