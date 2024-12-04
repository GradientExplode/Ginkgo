#include <gkg-graphics-X11-X11/X11FontFamilyImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-core/FontFamily.h>
#include <cstdio>


gkg::X11FontFamilyImplementation::X11FontFamilyImplementation(
                                                 gkg::FontFamily* fontFamily,
                                                 gkg::Display* display,
                                                 const std::string& familyName )
                                 : FontFamilyImplementation( fontFamily,
                                                             display,
                                                             familyName )
{

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                       display )->getXDisplay();
 
  std::string buffer = std::string( "*-*-" ) + familyName +
                       std::string( "-*-*-*-*-75-75-*-*-*-*" );
  char** fonts = XListFonts( xDisplay, buffer.c_str(), 100, &_count );

  _names = new std::string[ _count ];
  _weights = new int32_t[ _count ];
  _slants = new int32_t[ _count ];
  _widths = new int32_t[ _count ];
  _sizes = new int32_t[ _count ];
  _minWeight = 1000;
  _maxWeight = 0;
  _minSlant = 1000;
  _maxSlant = 0;
  _minWidth = 1000;
  _maxWidth = 0;
  _minSize = 1000;
  _maxSize = 0;
  for ( int i = 0; i < _count; i++ )
  {

    _names[ i ] = ( std::string )fonts[ i ];

    char weight[ 100 ];
    char slant[ 100 ];
    char width[ 100 ];
    int32_t size;
    sscanf( _names[ i ].c_str(), "-%*[^-]-%*[^-]-%[^-]-%[^-]-%[^-]--%*[^-]-%d",
            weight, slant, width, &size );
    _weights[ i ] = nameToValue( weight, _weightNames, 5 );

    std::string sl( slant );
    if ( ( sl == "o" ) || ( sl == "i" ) )
    {

      _slants[ i ] = 3;

    }
    else if ( sl == "r" )
    {

      _slants[ i ] = 2;

    }
    else if ( ( sl == "ro" ) || ( sl == "ri" ) )
    {

      _slants[ i ] = 1;

    }
    else
    {

      _slants[ i ] = 2;

    }

    _widths[ i ] = nameToValue( width, _widthNames, 5 );
    _sizes[ i ] = size / 10;

    _minWidth = std::min( _widths[ i ], _minWidth );
    _maxWidth = std::max( _widths[ i ], _maxWidth );
    _minWeight = std::min( _weights[ i ], _minWeight );
    _maxWeight = std::max( _weights[ i ], _maxWeight );
    _minSlant = std::min( _slants[ i ], _minSlant );
    _maxSlant = std::max( _slants[ i ], _maxSlant );
    _minSize = std::min( _sizes[ i ], _minSize );
    _maxSize = std::max( _sizes[ i ], _maxSize );

  }
  XFreeFontNames( fonts );

}


gkg::X11FontFamilyImplementation::~X11FontFamilyImplementation()
{
}



gkg::X11FontFamilyImplementation* 
gkg::X11FontFamilyImplementation::getImplementation(
                                                    gkg::FontFamily* fontFamily,
                                                    gkg::Display* display )
{

  return static_cast< gkg::X11FontFamilyImplementation* >(
                                     fontFamily->getImplementation( display ) );

}
