#include <gkg-graphics-X11-core/BrushImplementation.h>


gkg::BrushImplementation::BrushImplementation( const int32_t* /*pattern*/,
                                               int32_t /*count*/,
                                               gkg::PixelCoordinate pixelWidth,
                                               gkg::Display* display )
                         : _display( display ),
                           _pixelWidth( pixelWidth )
{
}


gkg::BrushImplementation::~BrushImplementation()
{
}


gkg::Display* gkg::BrushImplementation::getDisplay() const
{

  return _display;

}


gkg::PixelCoordinate gkg::BrushImplementation::getPixelWidth() const
{

  return _pixelWidth;

}


