#include <gkg-graphics-X11-core/BitmapImplementation.h>
#include <gkg-graphics-X11-core/Bitmap.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/Session.h>


gkg::BitmapImplementation::BitmapImplementation( 
                                        gkg::Bitmap* bitmap,
                                        const void* /*data*/,
                                        gkg::PixelCoordinate pixelWidth,
                                        gkg::PixelCoordinate pixelHeight,
                                        PixelCoordinate x0, PixelCoordinate y0 )
                          : _target( bitmap ),
                            _pixelWidth( pixelWidth ),
                            _pixelHeight( pixelHeight ),
                            _modified( false )
{

  _display = gkg::Session::getInstance().getDefaultDisplay();
  _width = _display->toFloatCoordinate( _pixelWidth );
  _height = _display->toFloatCoordinate( _pixelHeight );
  if ( x0 == -1 && y0 == -1 )
  {

    _left = 0;
    _right = _width;
    _bottom = 0;
    _top = _height;

  }
  else
  {

    _left = _display->toFloatCoordinate( -x0 );
    _right = _display->toFloatCoordinate( _pixelWidth - x0 );
    _bottom = _display->toFloatCoordinate( y0 - _pixelHeight );
    _top = _display->toFloatCoordinate( y0 );

  }

}


gkg::BitmapImplementation::BitmapImplementation( gkg::Bitmap* bitmap,
                                                 const gkg::Font*/*font*/,
                                                 int32_t /*character*/,
                                                 float /*scale*/ )
                          : _target( bitmap ),
                            _left( 0 ),
                            _bottom( 0 ),
                            _right( 0 ),
                            _top( 0 ),
                            _width( 0 ),
                            _height( 0 ),
                            _pixelWidth( 0 ),
                            _pixelHeight( 0 ),
                            _modified( false )
{

  _display = gkg::Session::getInstance().getDefaultDisplay();

}


gkg::BitmapImplementation::BitmapImplementation(
                                              gkg::Bitmap* bitmap,
                                              gkg::BitmapImplementation* other )
                          : _target( bitmap ),
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


gkg::BitmapImplementation::BitmapImplementation(
                                               gkg::Bitmap* bitmap,
                                               const std::string& /*filename*/ )
                          : _target( bitmap ),
                            _left( 0 ),
                            _bottom( 0 ),
                            _right( 0 ),
                            _top( 0 ),
                            _width( 0 ),
                            _height( 0 ),
                            _pixelWidth( 0 ),
                            _pixelHeight( 0 ),
                            _modified( false )
{

  _display = gkg::Session::getInstance().getDefaultDisplay();

}


gkg::BitmapImplementation::BitmapImplementation(
                                              gkg::Bitmap* bitmap,
                                              gkg::Display* display,
                                              gkg::FloatCoordinate left,
                                              gkg::FloatCoordinate bottom,
                                              gkg::FloatCoordinate right,
                                              gkg::FloatCoordinate top,
                                              gkg::FloatCoordinate width,
                                              gkg::FloatCoordinate height,
                                              gkg::PixelCoordinate pixelWidth,
                                              gkg::PixelCoordinate pixelHeight )
                          : _target( bitmap ),
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


gkg::BitmapImplementation::~BitmapImplementation()
{
}


gkg::Bitmap* gkg::BitmapImplementation::getTarget() const
{

  return _target;

}


gkg::Display* gkg::BitmapImplementation::getDisplay() const
{

  return _display;

}


gkg::FloatCoordinate gkg::BitmapImplementation::getWidth() const
{

  return _width;

}


gkg::FloatCoordinate gkg::BitmapImplementation::getHeight() const
{

  return _height;

}


gkg::FloatCoordinate gkg::BitmapImplementation::getLeft() const
{

  return _left;

}


gkg::FloatCoordinate gkg::BitmapImplementation::getBottom() const
{

  return _bottom;

}


gkg::FloatCoordinate gkg::BitmapImplementation::getRight() const
{

  return _right;

}


gkg::FloatCoordinate gkg::BitmapImplementation::getTop() const
{

  return _top;

}


gkg::PixelCoordinate gkg::BitmapImplementation::getPixelWidth() const
{

  return _pixelWidth;

}


gkg::PixelCoordinate gkg::BitmapImplementation::getPixelHeight() const
{

  return _pixelHeight;

}


gkg::FloatCoordinate gkg::BitmapImplementation::getLeftBearing() const
{

  return -_left;

}


gkg::FloatCoordinate gkg::BitmapImplementation::getRightBearing() const
{

  return _right;

}


gkg::FloatCoordinate gkg::BitmapImplementation::getAscent() const
{

  return _top;

}


gkg::FloatCoordinate gkg::BitmapImplementation::getDescent() const
{

  return -_bottom;

}
