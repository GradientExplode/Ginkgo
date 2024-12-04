#ifndef _gkg_graphics_X11_core_BackwardScroller_h_
#define _gkg_graphics_X11_core_BackwardScroller_h_


#include <gkg-graphics-X11-core/Stepper.h>


namespace gkg
{


class Adjustable;


class BackwardScroller : public Stepper
{

  public:

    BackwardScroller( Glyph* glyph,
                      Style* style,
                      TellTaleState* tellTaleState,
                      Adjustable* adjustable,
                      DimensionName dimensionName );
    virtual ~BackwardScroller();

    virtual void adjust();

  private:

    Adjustable* _adjustable;
    DimensionName _dimensionName;

};


}


#endif
