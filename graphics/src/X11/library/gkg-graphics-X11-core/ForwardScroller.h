#ifndef _gkg_graphics_X11_core_ForwardScroller_h_
#define _gkg_graphics_X11_core_ForwardScroller_h_


#include <gkg-graphics-X11-core/Stepper.h>


namespace gkg
{


class Adjustable;


class ForwardScroller : public Stepper
{

  public:

    ForwardScroller( Glyph* glyph,
                   Style* style,
                   TellTaleState* tellTaleState,
                   Adjustable* adjustable,
                   DimensionName dimensionName );
    virtual ~ForwardScroller();

    virtual void adjust();

  private:

    Adjustable* _adjustable;
    DimensionName _dimensionName;

};


}


#endif
