#include <gkg-graphics-X11-core/Brush.h>
#include <gkg-graphics-X11-core/BrushImplementation.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>
#include <cstring>


//
// class BrushImplementationLut
//

gkg::BrushImplementationLut::BrushImplementationLut(
                                          const int32_t* pattern, int32_t count,
                                          gkg::FloatCoordinate floatWidth )
                            : _floatWidth( floatWidth )
{

  if ( count > 0 && pattern )
  {

    _pattern = new int32_t[ count ];
    _count = count;
    memcpy( _pattern, pattern, _count * sizeof( int32_t ) );

  }
  else
  {

    _pattern = 0;
    _count = 0;

  }

}


gkg::BrushImplementationLut::~BrushImplementationLut()
{

  std::list< gkg::BrushImplementation* >::iterator
    b = _brushImplementations.begin(), be = _brushImplementations.end();
  while ( b != be )
  {

    delete *b;
    ++ b;

  }
  _brushImplementations.clear();

  if ( _pattern )
  {

    delete [] _pattern;

  }

}


gkg::FloatCoordinate gkg::BrushImplementationLut::getWidth() const
{

  return _floatWidth;

}


gkg::BrushImplementation* gkg::BrushImplementationLut::getImplementation(
                                                         gkg::Display* display )
{

  gkg::BrushImplementation* brushImpl = 0;

  std::list< gkg::BrushImplementation* >::iterator
    b = _brushImplementations.begin(), be = _brushImplementations.end(); 

  while ( b != be )
  {

    brushImpl = *b;
    if ( brushImpl->getDisplay() == display )
    {

      return brushImpl;

    }
    ++ b;

  }

  brushImpl = gkg::getGUIImplementationFactory()->createBrushImplementation( 
                                      _pattern,
                                      _count,
                                      display->toPixelCoordinate( _floatWidth ),
                                      display );
  _brushImplementations.push_back( brushImpl );

  return brushImpl;

}


//
// class Brush
//

gkg::Brush::Brush( gkg::FloatCoordinate floatWidth )
           : gkg::RCGUIObject(),
             _brushImplementationLut( new gkg::BrushImplementationLut(
                                                            0, 0, floatWidth ) )
{
}


gkg::Brush::Brush( const int32_t* pattern, int32_t count,
                   gkg::FloatCoordinate floatWidth )
           : gkg::RCGUIObject(),
             _brushImplementationLut( new gkg::BrushImplementationLut(
                                                  pattern, count, floatWidth ) )
{
}


gkg::Brush::Brush( int32_t patternVector, gkg::FloatCoordinate floatWidth )
           : gkg::RCGUIObject()
{

  int32_t pattern[ 16 ], count = 0;

  uint32_t p = patternVector & 0xffff;

  if ( p != 0 && p != 0xffff )
  {

    const uint32_t MSB = 1 << 15;
    while ( ( p & MSB ) == 0 )
      p <<= 1;

    if ( p == 0x5555 || p == 0xaaaa )
    {

      pattern[ 0 ] = 1;
      pattern[ 1 ] = 3;
      count = 2;

    }
    else if ( p == 0xaaaa )
    {

      pattern[ 0 ] = 1;
      pattern[ 1 ] = 1;
      count = 2;

    }
    else if ( p == 0xcccc )
    {

      pattern[ 0 ] = 2;
      pattern[ 1 ] = 2;
      count = 2;

    }
    else if ( p == 0xeeee )
    {

      pattern[ 0 ] = 3;
      pattern[ 1 ] = 1;
      count = 2;

    }
    else
    {

      uint32_t m = MSB;
      int32_t index = 0;
      while ( m != 0 )
      {

        // count the consecutive one bits
        int32_t length = 0;
        while ( m != 0 && ( p & m ) != 0 )
        {

          length ++;
          m >>= 1;

        }
        pattern[ index++ ] = length;

        // count the consecutive zero bits
        length = 0;	
        while ( m != 0 && ( p & m ) == 0 )
        {

          length ++;
          m >>= 1;

        }
        if ( length > 0 )
        {

          pattern[ index++ ] = length;

        }

      }
      count = index;

    }

    _brushImplementationLut = new gkg::BrushImplementationLut( pattern,
                                                               count,
                                                               floatWidth );

  }
  else
  {

    _brushImplementationLut = new gkg::BrushImplementationLut( 0,
                                                               0,
                                                               floatWidth );

  }

}


gkg::Brush::~Brush()
{

  delete _brushImplementationLut;

}


gkg::FloatCoordinate gkg::Brush::getWidth() const
{

  return _brushImplementationLut->getWidth();

}


gkg::BrushImplementation* 
gkg::Brush::getImplementation( gkg::Display* display ) const
{

  return _brushImplementationLut->getImplementation( display );

}
