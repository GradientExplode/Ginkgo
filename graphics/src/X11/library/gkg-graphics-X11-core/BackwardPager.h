#ifndef _gkg_graphics_X11_core_BackwardPager_h_
#define _gkg_graphics_X11_core_BackwardPager_h_


#include <gkg-graphics-X11-core/Stepper.h>


namespace gkg
{


class Adjustable;


class BackwardPager : public Stepper
{

  public:

    BackwardPager( Glyph* glyph,
                   Style* style,
                   TellTaleState* tellTaleState,
                   Adjustable* adjustable,
                   DimensionName dimensionName );
    virtual ~BackwardPager();

    virtual void adjust();

  private:

    Adjustable* _adjustable;
    DimensionName _dimensionName;

};


}


#endif
