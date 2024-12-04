#include <gkg-graphics-X11-core/FontFamilyImplementation.h>
#include <gkg-graphics-X11-core/FontFamily.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-processing-algobase/Math.h>


static gkg::FontFamilyImplementation::FontNameSet weightNames[] =
{

  { 1, { "ultralight", 0 } },
  { 2, { "extralight", 0 } },
  { 3, { "light", 0 } },
  { 4, { "semilight", "book", 0 } },
  { 5, { "medium", "normal", "regular", 0 } },
  { 6, { "semibold", "demibold", 0 } },
  { 7, { "bold", 0 } },
  { 8, { "extrabold", "heavy", 0 } },
  { 9, { "ultrabold", "black", 0 } },
  { 0, { 0 } }

};


static gkg::FontFamilyImplementation::FontNameSet slantNames[] =
{

  { 1, { "reverseitalic", "reverseoblique", 0 } },
  { 2, { "roman", "normal", "regular", 0 } },
  { 3, { "italic", "oblique", 0 } },
  { 0, { 0 } }

};


static gkg::FontFamilyImplementation::FontNameSet widthNames[] =
{

  { 1, { "ultracondensed", 0 } },
  { 2, { "extracondensed", 0 } },
  { 3, { "condensed", 0 } },
  { 4, { "semicondensed", 0 } },
  { 5, { "medium", "normal", "regular", 0 } },
  { 6, { "semiexpanded", "demiexpanded", 0 } },
  { 7, { "expanded", 0 } },
  { 8, { "extraexpanded", "wide", 0 } },
  { 9, { "ultraexpanded", "extrawide", 0 } },
  { 0, { 0 } }

};


gkg::FontFamilyImplementation::FontNameSet*
gkg::FontFamilyImplementation::_weightNames = weightNames;


gkg::FontFamilyImplementation::FontNameSet*
gkg::FontFamilyImplementation::_slantNames = slantNames;


gkg::FontFamilyImplementation::FontNameSet*
gkg::FontFamilyImplementation::_widthNames = widthNames;


gkg::FontFamilyImplementation::FontFamilyImplementation(
                                             gkg::FontFamily* fontFamily,
                                             gkg::Display* display,
                                             const std::string& /*familyName*/ )
                              : _target( fontFamily ),
                                _display( display ),
                                _count( 0 ),
                                _minWeight( 0 ),
                                _maxWeight( 0 ),
                                _minWidth( 0 ),
                                _maxWidth( 0 ),
                                _minSlant( 0 ),
                                _maxSlant( 0 ),
                                _minSize( 0 ),
                                _maxSize( 0 ),
                                _names( 0 ),
                                _weights( 0 ),
                                _slants( 0 ),
                                _widths( 0 ),
                                _sizes( 0 )
{
}


gkg::FontFamilyImplementation::~FontFamilyImplementation()
{

  delete [] _names;
  delete [] _weights;
  delete [] _slants;
  delete [] _widths;
  delete [] _sizes;

}


gkg::FontFamily* gkg::FontFamilyImplementation::getTarget() const
{

  return _target;

}


gkg::Display* gkg::FontFamilyImplementation::getDisplay() const
{

  return _display;

}


int32_t gkg::FontFamilyImplementation::getCount() const
{

  return _count;

}


int32_t gkg::FontFamilyImplementation::getMinWeight() const
{

  return _minWeight;

}


int32_t gkg::FontFamilyImplementation::getMaxWeight() const
{

  return _maxWeight;

}


int32_t gkg::FontFamilyImplementation::getMinWidth() const
{

  return _minWidth;

}


int32_t gkg::FontFamilyImplementation::getMaxWidth() const
{

  return _maxWidth;

}


int32_t gkg::FontFamilyImplementation::getMinSlant() const
{

  return _minSlant;

}


int32_t gkg::FontFamilyImplementation::getMaxSlant() const
{

  return _maxSlant;

}


int32_t gkg::FontFamilyImplementation::getMinSize() const
{

  return _minSize;

}


int32_t gkg::FontFamilyImplementation::getMaxSize() const
{

  return _maxSize;

}



std::string* gkg::FontFamilyImplementation::getNames() const
{

  return _names;

}


int32_t* gkg::FontFamilyImplementation::getWeights() const
{

  return _weights;

}


int32_t* gkg::FontFamilyImplementation::getSlants() const
{

  return _slants;

}


int32_t* gkg::FontFamilyImplementation::getWidths() const
{

  return _widths;

}


int32_t* gkg::FontFamilyImplementation::getSizes() const
{

  return _sizes;

}


bool 
gkg::FontFamilyImplementation::getFont( int32_t size, const std::string& style,
                                        std::string& name, float& scale )
{

  int32_t weight = nameToValue( style, _weightNames, 5 );
  int32_t slant = nameToValue( style, _slantNames, 2 );
  int32_t width = nameToValue( style, _widthNames, 5 );

  int32_t bestMatch = -1;
  int32_t leastBadness = 1000;

  for ( int32_t i = 0; i < _count; i++ )
  {

    int32_t badness = 0;
    badness += gkg::abs( _weights[ i ] - weight );
    badness += gkg::abs( _widths[ i ] - width );
    badness += gkg::abs( _slants[ i ] - slant );
    badness += gkg::abs( _sizes[ i ] - size );
    if ( badness < leastBadness )
    {

      leastBadness = badness;
      bestMatch = i;

    }

  }
  if ( bestMatch == -1 )
  {

    return false;

  }
  int32_t bestSize = _sizes[ bestMatch ];
  name = _names[ bestMatch ];
  scale = ( size == bestSize ) ? 1.0 : ( float )size / ( float )bestSize;
  return true;

}


int32_t gkg::FontFamilyImplementation::nameToValue(
                             const std::string& name,
                             gkg::FontFamilyImplementation::FontNameSet* values,
                             int32_t def )
{

  int32_t i = 0;
  while ( values[ i ].value )
  {

    int32_t j = 0;
    while ( values[ i ].names[ j ] )
    {

      if ( name.find( values[ i ].names[ j ] ) )
      {

        return values[ i ].value;

      }
      ++ j;

    }
    ++ i;

  }
  return def;

}

