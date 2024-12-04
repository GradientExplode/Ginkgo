#ifndef _gkg_graphics_X11_core_DragBox_h_
#define _gkg_graphics_X11_core_DragBox_h_


#include <gkg-graphics-X11-core/Background.h>


namespace gkg
{


class DragBox : public Background
{

  public:

    DragBox( Glyph* body, const Color* color );
    virtual ~DragBox();

    virtual void setEverDrawn( bool drawn );
    virtual bool isEverDrawn() const;

    virtual void draw( Canvas* canvas, 
                       const GeometryAllocation& geometryAllocation ) const;

  protected:

    bool _everDrawn;

};


}


#endif
