#ifndef _gkg_graphics_X11_core_ForwardPager_h_
#define _gkg_graphics_X11_core_ForwardPager_h_


#include <gkg-graphics-X11-core/Stepper.h>


namespace gkg
{


class Adjustable;


class ForwardPager : public Stepper
{

  public:

    ForwardPager( Glyph* glyph,
                  Style* style,
                  TellTaleState* tellTaleState,
                  Adjustable* adjustable,
                  DimensionName dimensionName );
    virtual ~ForwardPager();

    virtual void adjust();

  private:

    Adjustable* _adjustable;
    DimensionName _dimensionName;

};


}


#endif
