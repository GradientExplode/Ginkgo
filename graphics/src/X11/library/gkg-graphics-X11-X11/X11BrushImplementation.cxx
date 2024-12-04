#include <gkg-graphics-X11-X11/X11BrushImplementation.h>
#include <gkg-graphics-X11-core/Brush.h>


gkg::X11BrushImplementation::X11BrushImplementation( 
                                          const int32_t* pattern, int32_t count,
                                          gkg::PixelCoordinate pixelWidth,
                                          gkg::Display* display )
                            : gkg::BrushImplementation( pattern,
                                                        count,
                                                        pixelWidth,
                                                        display ),
                              _dashList( 0 ),
                              _dashCount( 0 )
{

  if ( count > 0 )
  {

    _dashCount = count;
    _dashList = new char[ count ];
    for ( int32_t c = 0; c < count; c++ )
    {

      _dashList[ c ] = ( char )pattern[ c ];

    }

  }

}


gkg::X11BrushImplementation::~X11BrushImplementation()
{

  if ( _dashList )
  {

    delete [] _dashList;

  }

}


char* gkg::X11BrushImplementation::getDashList() const
{

  return _dashList;

}


int32_t gkg::X11BrushImplementation::getDashCount() const
{

  return _dashCount;

}


gkg::X11BrushImplementation*
gkg::X11BrushImplementation::getImplementation( gkg::Brush* brush,
                                                gkg::Display* display )
{

  return static_cast< X11BrushImplementation* >(
                                          brush->getImplementation( display ) );

}
