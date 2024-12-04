#include <gkg-graphics-X11-X11/X11FontImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/Display.h>
#include <X11/Xatom.h>


gkg::X11FontImplementation::X11FontImplementation( gkg::Display* display,
                                                   const std::string& name,
                                                   float scale )
                           : gkg::FontImplementation( display,
                                                      name,
                                                      scale )
{

  static Atom XA_CHARSET_REGISTRY = 0;

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                       display )->getXDisplay();
  _xFontStruct = XLoadQueryFont( xDisplay, name.c_str() );

  if ( !_xFontStruct )
  {

    _xFontStruct = XLoadQueryFont( 
                                  xDisplay, 
                                  "-*-fixed-medium-r-normal--*-120-*-*-*-*-*" );

  }

  unsigned long value; // Do not modify since it is architecture dependent
  if ( XGetFontProperty( _xFontStruct, XA_FULL_NAME, &value ) ||
       XGetFontProperty( _xFontStruct, XA_FAMILY_NAME, &value ) )
  {

    char* fullName = XGetAtomName( xDisplay, ( Atom )value );
    _name = std::string( fullName );
    XFree( fullName );

  }
  else
  {

    _name = name;

  }

  if ( XA_CHARSET_REGISTRY == 0 )
  {

    XA_CHARSET_REGISTRY = XInternAtom( xDisplay, "CHARSET_REGISTRY", False );

  }
  if ( XGetFontProperty( _xFontStruct, XA_CHARSET_REGISTRY, &value ) )
  {

    char* registry = XGetAtomName( xDisplay, ( Atom )value );
    _encoding = std::string( registry );
    XFree( registry );

  }
  else
  {

    _encoding = "";

  }

  if ( XGetFontProperty( _xFontStruct, XA_POINT_SIZE, &value ) )
  {

    _size = ( float )( value / 10 ) * _scale;

  }
  else
  {

    _size = 0;

  }

}


gkg::X11FontImplementation::~X11FontImplementation() 
{

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
  XFreeFont( xDisplay, _xFontStruct );

}



void gkg::X11FontImplementation::getFontBoundingBox(
                                              gkg::FontBoundingBox& bbox ) const
{

  bbox.leftBearing = _scale * _display->toFloatCoordinate(
                                       _xFontStruct->max_bounds.lbearing );
  bbox.rightBearing = _scale * _display->toFloatCoordinate(
                                       _xFontStruct->max_bounds.rbearing );
  bbox.width = _scale *
               _display->toFloatCoordinate( _xFontStruct->max_bounds.width );
  bbox.ascent = _scale * _display->toFloatCoordinate( _xFontStruct->ascent );
  bbox.descent = _scale * _display->toFloatCoordinate( _xFontStruct->descent );
  bbox.fontAscent = bbox.ascent;
  bbox.fontDescent = bbox.descent;

}


void gkg::X11FontImplementation::getCharBoundingBox( 
                                              int32_t c,
                                              gkg::FontBoundingBox& bbox ) const
{

  if ( c < 0 )
  {

    bbox.leftBearing = 0;
    bbox.rightBearing = 0;
    bbox.width = 0;
    bbox.ascent = 0;
    bbox.descent = 0;
    bbox.fontAscent = 0;
    bbox.fontDescent = 0;
    return;

  }
  XCharStruct xCharStruct;
  XChar2b xChar2b;
  xChar2b.byte1 = ( uint8_t )( ( c & 0xff00 ) >> 8 );
  xChar2b.byte2 = ( uint8_t )( c & 0xff );
  int32_t dir, asc, des;
  XTextExtents16( _xFontStruct, &xChar2b, 1, &dir, &asc, &des, &xCharStruct );
  bbox.leftBearing = _scale *
                     _display->toFloatCoordinate( -xCharStruct.lbearing );
  bbox.rightBearing = _scale *
                      _display->toFloatCoordinate( xCharStruct.rbearing );
  bbox.width = getWidth( c );
  bbox.ascent = _scale * _display->toFloatCoordinate( xCharStruct.ascent );
  bbox.descent = _scale * _display->toFloatCoordinate( xCharStruct.descent );
  bbox.fontAscent = _scale *
                    _display->toFloatCoordinate( _xFontStruct->ascent );
  bbox.fontDescent = _scale *
                     _display->toFloatCoordinate( _xFontStruct->descent );

}


void gkg::X11FontImplementation::getStringBoundingBox( 
                                              const std::string& s,
                                              gkg::FontBoundingBox& bbox ) const
{

  XCharStruct xCharStruct;
  int32_t dir, asc, des;
  XTextExtents( _xFontStruct, s.c_str(), s.length(), &dir, &asc, &des,
                &xCharStruct );
  bbox.leftBearing = _scale *
                     _display->toFloatCoordinate( -xCharStruct.lbearing );
  bbox.rightBearing = _scale *
                      _display->toFloatCoordinate( xCharStruct.rbearing );
  bbox.width = getWidth( s );
  bbox.ascent = _scale * _display->toFloatCoordinate( xCharStruct.ascent );
  bbox.descent = _scale * _display->toFloatCoordinate( xCharStruct.descent );
  bbox.fontAscent = _scale *
                    _display->toFloatCoordinate( _xFontStruct->ascent );
  bbox.fontDescent = _scale *
                     _display->toFloatCoordinate( _xFontStruct->descent );

}


gkg::FloatCoordinate gkg::X11FontImplementation::getWidth( int32_t c ) const
{

  if ( c < 0 )
  {

    return 0;

  }
  XChar2b xChar2b;
  xChar2b.byte1 = ( uint8_t )( ( c & 0xff00 ) >> 8 );
  xChar2b.byte2 = ( uint8_t )( c & 0xff );
  return _scale * _display->toFloatCoordinate( XTextWidth16( _xFontStruct,
                                                             &xChar2b,
                                                             1 ) );

}


gkg::FloatCoordinate 
gkg::X11FontImplementation::getWidth( const std::string& s ) const
{

  return _scale * _display->toFloatCoordinate( XTextWidth( _xFontStruct,
                                                           s.c_str(),
                                                           s.length() ) );

}


int32_t gkg::X11FontImplementation::getIndex( const std::string& s,
                                              float offset, 
                                              bool between ) const
{

  int32_t n, w;
  int32_t coff, cw = 0;

  if ( ( offset < 0 ) || s.empty() )
  {

    return 0;

  }
  int32_t xOffset = _display->toPixelCoordinate( 
                                       ( FloatCoordinate )( offset * _scale ) );
  if ( _xFontStruct->min_bounds.width == _xFontStruct->max_bounds.width )
  {

    cw = _xFontStruct->min_bounds.width;
    n = xOffset / cw;
    coff = xOffset % cw;

  }
  else
  {

    w = 0;
    const char* p;
    for ( p = s.c_str(), n = 0; 
          *p != '\0' && n < ( int32_t )s.length(); 
          p++, n++ )
    {

      cw = XTextWidth( _xFontStruct, p, 1 );
      w += cw;
      if ( w > xOffset )
      {

        break;

      }

    }
    coff = xOffset - w + cw;

  }
  if ( between && ( coff > cw / 2 ) )
  {

    ++ n;

  }
  return std::min( n, ( int32_t )s.length() );

}


XFontStruct* gkg::X11FontImplementation::getXFontStruct() const
{

  return _xFontStruct;

}


gkg::X11FontImplementation* 
gkg::X11FontImplementation::getImplementation( gkg::Font* font,
                                               gkg::Display* display )
{

  return static_cast< gkg::X11FontImplementation* >(
                                           font->getImplementation( display ) );

}


