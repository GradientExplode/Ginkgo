#include <gkg-graphics-X11-core/RasterImplementation.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/Display.h>


gkg::RasterImplementation::RasterImplementation( 
                                              gkg::Raster* raster,
                                              gkg::PixelCoordinate pixelWidth,
                                              gkg::PixelCoordinate pixelHeight )
                          : _target( raster ),
                            _pixelWidth( pixelWidth ),
                            _pixelHeight( pixelHeight ),
                            _modified( false )
{

  _display = gkg::Session::getInstance().getDefaultDisplay();
  _width = _display->toFloatCoordinate( _pixelWidth );
  _height = _display->toFloatCoordinate( _pixelHeight );
  _left = 0;
  _right = _width;
  _bottom = 0;
  _top = _height;

}


gkg::RasterImplementation::RasterImplementation(
                                              gkg::Raster* raster,
                                              gkg::RasterImplementation* other )
                          : _target( raster ),
                            _display( other->_display ),
                            _left( other->_left ),
                            _bottom( other->_bottom ),
                            _right( other->_right ),
                            _top( other->_top ),
                            _width( other->_width ),
                            _height( other->_height ),
                            _pixelWidth( other->_pixelWidth ),
                            _pixelHeight( other->_pixelHeight ),
                            _modified( true )
{
}


gkg::RasterImplementation::RasterImplementation(
                                              gkg::Raster* raster,
                                              gkg::Display* display,
                                              gkg::FloatCoordinate left,
                                              gkg::FloatCoordinate bottom,
                                              gkg::FloatCoordinate right,
                                              gkg::FloatCoordinate top,
                                              gkg::FloatCoordinate width,
                                              gkg::FloatCoordinate height,
                                              gkg::PixelCoordinate pixelWidth,
                                              gkg::PixelCoordinate pixelHeight )
                          : _target( raster ),
                            _display( display ),
                            _left( left ),
                            _bottom( bottom ),
                            _right( right ),
                            _top( top ),
                            _width( width ),
                            _height( height ),
                            _pixelWidth( pixelWidth ),
                            _pixelHeight( pixelHeight ),
                            _modified( false )
{
}


gkg::RasterImplementation::~RasterImplementation()
{
}


gkg::Raster* gkg::RasterImplementation::getTarget() const
{

  return _target;

}


gkg::Display* gkg::RasterImplementation::getDisplay() const
{

  return _display;

}


gkg::FloatCoordinate gkg::RasterImplementation::getWidth() const
{

  return _width;

}


gkg::FloatCoordinate gkg::RasterImplementation::getHeight() const
{

  return _height;

}


gkg::FloatCoordinate gkg::RasterImplementation::getLeft() const
{

  return _left;

}


gkg::FloatCoordinate gkg::RasterImplementation::getBottom() const
{

  return _bottom;

}


gkg::FloatCoordinate gkg::RasterImplementation::getRight() const
{

  return _right;

}


gkg::FloatCoordinate gkg::RasterImplementation::getTop() const
{

  return _top;

}


gkg::PixelCoordinate gkg::RasterImplementation::getPixelWidth() const
{

  return _pixelWidth;

}


gkg::PixelCoordinate gkg::RasterImplementation::getPixelHeight() const
{

  return _pixelHeight;

}


gkg::FloatCoordinate gkg::RasterImplementation::getLeftBearing() const
{

  return -_left;

}


gkg::FloatCoordinate gkg::RasterImplementation::getRightBearing() const
{

  return _right;

}


gkg::FloatCoordinate gkg::RasterImplementation::getAscent() const
{

  return _top;

}


gkg::FloatCoordinate gkg::RasterImplementation::getDescent() const
{

  return -_bottom;

}
