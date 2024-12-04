#include <gkg-graphics-X11-X11/X11ColorImplementation.h>
#include <gkg-graphics-X11-X11/X11Visual.h>
#include <gkg-processing-algobase/Math.h>


static char stippleData[][ 4 ] =
{

  { '\000', '\000', '\000', '\000' },
  { '\001', '\000', '\000', '\000' },
  { '\001', '\000', '\004', '\000' },
  { '\005', '\000', '\004', '\000' },
  { '\005', '\000', '\005', '\000' },
  { '\005', '\002', '\005', '\000' },
  { '\005', '\002', '\005', '\010' },
  { '\005', '\012', '\005', '\010' },
  { '\005', '\012', '\005', '\012' },
  { '\007', '\012', '\005', '\012' },
  { '\007', '\012', '\015', '\012' },
  { '\017', '\012', '\015', '\012' },
  { '\017', '\012', '\017', '\012' },
  { '\017', '\013', '\017', '\012' },
  { '\017', '\013', '\017', '\016' },
  { '\017', '\017', '\017', '\016' }

};


Pixmap gkg::X11ColorImplementation::_stipples[ 16 ] = 
{

  None, None, None, None, None, None, None, None, 
  None, None, None, None, None, None, None, None

};


gkg::X11ColorImplementation::X11ColorImplementation(  
                                               gkg::Visual* visual,
                                               gkg::FloatColorIntensity r,
                                               gkg::FloatColorIntensity g,
                                               gkg::FloatColorIntensity b,
                                               float alpha,
                                               gkg::Color::Operation operation )
                            : gkg::ColorImplementation( visual )
{

  uint16_t red = ( uint16_t )gkg::round( r * ( float )0xffff );
  uint16_t green = ( uint16_t )gkg::round( g * ( float )0xffff );
  uint16_t blue = ( uint16_t )gkg::round( b * ( float )0xffff );

  gkg::X11Visual* x11Visual = gkg::X11Visual::getImplementation( visual );
  x11Visual->findColor( red, green, blue, _xColor );

  switch( operation )
  {

    case gkg::Color::Copy:

      _operation = GXcopy;
      _masking = false;
      break;

    case gkg::Color::Xor:

      _operation = GXxor;
      _masking = false;
      break;

    case gkg::Color::Invisible:

      _operation = GXnoop;
      _masking = false;
      break;

  }
  _stipple = ( alpha > 0.9999 && alpha < 1.0001 ) ? None :
                                                    makeStipple( alpha );

}


gkg::X11ColorImplementation::~X11ColorImplementation() 
{
}


const XColor& gkg::X11ColorImplementation::getXColor() const
{

  return _xColor;

}


int32_t gkg::X11ColorImplementation::getOperation() const
{

  return _operation;

}


Pixmap gkg::X11ColorImplementation::getStipple() const
{

  return _stipple;

}


bool gkg::X11ColorImplementation::distinguished( const Color* color ) const
{

  gkg::X11ColorImplementation* x11ColorImpl = 
    gkg::X11ColorImplementation::getImplementation( ( gkg::Color* )color,
                                                    _visual );

  const XColor& xColor = x11ColorImpl->getXColor();

  return ( _xColor.red   != xColor.red )   ||
         ( _xColor.green != xColor.green ) ||
         ( _xColor.blue  != xColor.blue );
         
}


void gkg::X11ColorImplementation::getIntensities(
                                             gkg::FloatColorIntensity& r,
                                             gkg::FloatColorIntensity& g,
                                             gkg::FloatColorIntensity& b ) const
{

  r = ( float )_xColor.red / ( float )0xffff;
  g = ( float )_xColor.green / ( float )0xffff;
  b = ( float )_xColor.blue / ( float )0xffff;

}


gkg::X11ColorImplementation* 
gkg::X11ColorImplementation::getImplementation( gkg::Color* color,
                                                gkg::Visual* visual )
{

  return static_cast< gkg::X11ColorImplementation* >(
                                           color->getImplementation( visual ) );

}


Pixmap gkg::X11ColorImplementation::makeStipple( float alpha )
{

  int32_t index = std::max( 0, std::min( ( int32_t )( alpha * 16 ), 15 ) );
  if ( _stipples[ index ] == None )
  {

    gkg::X11Visual* 
      x11Visual = gkg::X11Visual::getImplementation( _visual );

    _stipples[ index ] = XCreateBitmapFromData( 
                               x11Visual->getXDisplay(),
                               RootWindow( x11Visual->getXDisplay(),
                                           x11Visual->getScreen() ),
	                       stippleData[ index ], 4, 4 );

  }
  return _stipples[ index ];

}
