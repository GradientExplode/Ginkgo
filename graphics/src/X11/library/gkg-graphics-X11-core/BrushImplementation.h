#ifndef _gkg_graphics_X11_core_BrushImplementation_h_
#define _gkg_graphics_X11_core_BrushImplementation_h_


#include <gkg-graphics-X11-base/Coordinates.h>


namespace gkg
{


class Display;
class Brush;


class BrushImplementation
{

  public:

    virtual ~BrushImplementation();

    virtual Display* getDisplay() const;
    virtual PixelCoordinate getPixelWidth() const;

  protected:

    BrushImplementation( const int32_t* pattern, int32_t count,
                         PixelCoordinate pixelWidth,
                         Display* display );

    Display* _display;
    PixelCoordinate _pixelWidth;    

};


}


#endif
