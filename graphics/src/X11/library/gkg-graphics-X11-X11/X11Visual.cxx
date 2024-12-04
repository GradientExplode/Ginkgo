#include <gkg-graphics-X11-X11/X11Visual.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Display.h>


static gkg::VisualTable visualClasses[] =
{

  { "TrueColor",   TrueColor   },
  { "PseudoColor", PseudoColor },
  { "StaticGray",  StaticGray  },
  { "GrayScale",   GrayScale   },
  { "StaticColor", StaticColor },
  { "DirectColor", DirectColor },
  { 0, -1 }

};


gkg::X11Visual::X11Visual( const gkg::X11VisualInfo& x11VisualInfo )
               : Visual(),
                 _x11VisualInfo( x11VisualInfo ) 
{

  if ( !_x11VisualInfo.xVisual )
  {

    _x11VisualInfo.xVisual = DefaultVisual( _x11VisualInfo.xDisplay,
                                            _x11VisualInfo.screen );
    _x11VisualInfo.cmap = DefaultColormap( _x11VisualInfo.xDisplay,
                                           _x11VisualInfo.screen );

  }
  else
  {

    _x11VisualInfo.cmap = XCreateColormap( _x11VisualInfo.xDisplay,
                                           RootWindow( _x11VisualInfo.xDisplay,
                                                       _x11VisualInfo.screen ),
                                           _x11VisualInfo.xVisual, AllocNone );

  }
  initColorTables();

}


gkg::X11Visual::~X11Visual()
{

  delete _colorTable;
  delete _rgbTable;
  delete [] _localMap;

}


// Set up appropriate color mapping tables for the visual.
// For TrueColor, we don't need an rgb->pixel table because we can
// compute the pixel value directly.  The pixel->rgb table (ctable_)
// is used by Raster::peek.
//
void gkg::X11Visual::initColorTables()
{

  _colorTable = new gkg::ColorTable;
  _localMap = 0;
  _localMapSize = 0;
  XVisual* xVisual = _x11VisualInfo.xVisual;
  switch ( xVisual->c_class )
  {

    case TrueColor:

      _rgbTable = 0;
      setShift( xVisual->red_mask,   _red,   _redShift   );
      setShift( xVisual->green_mask, _green, _greenShift );
      setShift( xVisual->blue_mask,  _blue,  _blueShift  );
      break;

    default:

      _rgbTable = new gkg::RGBTable;
      if ( ( xVisual->c_class == PseudoColor ) &&
           ( xVisual->map_entries < 16 ) )
      {

        XColor xColor;
        findColor( 0, 0, 0, xColor );
        findColor( 0xffff, 0xffff, 0xffff, xColor );

      }
      break;

  }

}


bool gkg::X11Visual::findColor( const std::string& name, 
                                gkg::FloatColorIntensity& r,
                                gkg::FloatColorIntensity& g,
                                gkg::FloatColorIntensity& b ) const
{

  XColor xColor;
  if ( XParseColor( _x11VisualInfo.xDisplay,
                    _x11VisualInfo.cmap,
                    name.c_str(),
                    &xColor ) )
  {

    r = ( float )xColor.red / ( float )0xffff;
    g = ( float )xColor.green / ( float )0xffff;
    b = ( float )xColor.blue / ( float )0xffff;
    return true;

  }
  return false;

}


// Find the X color information for a given pixel value.
// If it is already in the color table, just retrieve.
// Otherwise, we have to query X.
void gkg::X11Visual::findColor( uint32_t pixel, XColor& xColor )
{

  gkg::ColorTable::const_iterator c = _colorTable->find( pixel );
  if ( c == _colorTable->end() )
  {

    xColor.pixel = pixel;
    XQueryColor( _x11VisualInfo.xDisplay, _x11VisualInfo.cmap, &xColor );
    ( *_colorTable )[ pixel ] =  xColor;

  }
  else
  {

    xColor = ( *c ).second;

  }

}


// Find the X color information for a specified rgb.
// For a TrueColor visual, this is easy (computed directly from rgb values).
// Otherwise, we have to do an XAllocColor if we haven't seen the rgb
// combination before.  If XAllocColor fails, then we read the colormap and
// try to find the best match.  Note this may cause havoc if the colormap
// entries are read/write.
void gkg::X11Visual::findColor( uint16_t r,
                                uint16_t g,
                                uint16_t b,
                                XColor& xColor )
{

  uint32_t red, green, blue;
  switch ( _x11VisualInfo.xVisual->c_class )
  {

    case TrueColor:

      red   = rescale( r, 0xffff, _red );
      green = rescale( g, 0xffff, _green );
      blue  = rescale( b, 0xffff, _blue );
      xColor.pixel = ( red   << _redShift   ) |
                     ( green << _greenShift ) |
                     ( blue  << _blueShift  );
      xColor.red   = ( uint16_t )rescale( red,   _red,   0xffff );
      xColor.green = ( uint16_t )rescale( green, _green, 0xffff );
      xColor.blue  = ( uint16_t )rescale( blue,  _blue,  0xffff );
      break;

    default:

      gkg::RGB rgb( r, g, b );
      gkg::RGBTable::const_iterator c = _rgbTable->find( rgb );
      if ( c == _rgbTable->end() )
      {

        if ( _localMapSize == 0 )
        {

          xColor.red   = r;
          xColor.green = g;
          xColor.blue  = b;
          if ( !XAllocColor( _x11VisualInfo.xDisplay,
                             _x11VisualInfo.cmap,
                             &xColor ) )
          {

            _localMapSize = std::min( _x11VisualInfo.xVisual->map_entries,
                                      256 );
            _localMap = new XColor[ _localMapSize ];
            for ( uint32_t p = 0; p < _localMapSize; p++ )
            {

              _localMap[ p ].pixel = p;

            }
            XQueryColors( _x11VisualInfo.xDisplay, _x11VisualInfo.cmap,
                          _localMap, _localMapSize );

          }

	}
        else
        {

          uint32_t best = 0;
          double bestMatch = 0.0;
          bool matched = false;
          for ( int32_t p = 0; p < ( int32_t )_localMapSize; p++ )
          {

            const gkg::X11OverlayInfo& overlay = _x11VisualInfo.overlay;
            if ( overlay.id == 0x0 || overlay.transparent != p )
            {

              double d = distance( r, g, b, _localMap[ p ] );
              if ( !matched || d < bestMatch )
              {

                best = p;
                bestMatch = d;
                matched = true;

              }

            }

          }
          xColor = _localMap[ best ];

        }
        ( *_rgbTable )[ rgb ] = xColor;

      }
      else
      {

        xColor = ( *c ).second;

      }

  }

}


uint32_t gkg::X11Visual::getXor( const gkg::Style& style ) const
{

  uint32_t p = 1;
  std::string custom;
  if ( style.findAttribute( "RubberbandPixel", custom ) )
  {

    gkg::StringConverter::toScalar( custom, p );

  }
  else if ( _x11VisualInfo.xVisual->c_class == DirectColor )
  {

    p = ( MSB( _x11VisualInfo.xVisual->red_mask   ) |
          MSB( _x11VisualInfo.xVisual->green_mask ) |
          MSB( _x11VisualInfo.xVisual->blue_mask  ) );

  }
  else
  {

    XDisplay* xDisplay = _x11VisualInfo.xDisplay;
    int32_t screen = _x11VisualInfo.screen;
    p = BlackPixel( xDisplay, screen ) ^ WhitePixel( xDisplay, screen );

  }
  return p;

}


XDisplay* gkg::X11Visual::getXDisplay() const
{

  return _x11VisualInfo.xDisplay;

}


int32_t gkg::X11Visual::getScreen() const
{

  return _x11VisualInfo.screen;

}


int32_t gkg::X11Visual::getDepth() const
{

  return _x11VisualInfo.depth;

}


XVisual* gkg::X11Visual::getXVisual() const
{

  return _x11VisualInfo.xVisual;

}


XColormap gkg::X11Visual::getColormap() const
{

  return _x11VisualInfo.cmap;

}


const gkg::X11VisualInfo& gkg::X11Visual::getX11VisualInfo() const
{

  return _x11VisualInfo;

}


gkg::Visual* gkg::X11Visual::findVisual( gkg::Display* display,
                                         gkg::Style* style )
{

  X11VisualInfo x11VisualInfo;

  gkg::X11DisplayImplementation* x11DisplayImpl =
    gkg::X11DisplayImplementation::getImplementation( display );

  gkg::X11Visual* x11Visual =
    gkg::X11Visual::getImplementation( x11DisplayImpl->getDefaultVisual() );

  const std::list< gkg::Visual* >&
    visuals = display->getImplementation()->getVisuals();

  x11VisualInfo.xDisplay = x11DisplayImpl->getXDisplay();
  x11VisualInfo.screen = x11DisplayImpl->getScreen();
  x11VisualInfo.depth = DefaultDepth( x11VisualInfo.xDisplay, 
                                      x11VisualInfo.screen );
  x11VisualInfo.xVisual = visuals.empty() ? 0 : x11Visual->getXVisual();
  x11VisualInfo.overlay.id = 0x0;

  std::string s;
  int32_t layer;
  if ( style->findAttribute( "visual_id", s ) )
  {

    int32_t id;
    if ( gkg::StringConverter::toScalar( s, id ) )
    {

      XVisualInfo xVisualInfo;
      xVisualInfo.visualid = id;
      findVisualByInfo( xVisualInfo, VisualIDMask, x11VisualInfo );

    }

  }
  else if ( style->findAttribute( "visual", s ) )
  {

    findVisualByClassName( s, x11VisualInfo );

  }
  else if ( style->findAttribute( "overlay", s ) && findLayer( s, layer ) )
  {

    std::list< gkg::Visual* >::const_iterator v = visuals.begin(),
                                              ve = visuals.end();

    while ( v != ve )
    {

      x11Visual = gkg::X11Visual::getImplementation( *v );
      const X11OverlayInfo& overlay = x11Visual->getX11VisualInfo().overlay;
      if ( overlay.id != 0x0 &&
           ( ( layer == 0 ) || ( overlay.layer == layer ) ) )
      {

        return *v;

      }
      ++ v;

    }
    findOverlay( layer, x11VisualInfo );

  }

  gkg::Visual* visual = 0;
  {

    std::list< gkg::Visual* >::const_iterator v = visuals.begin(),
                                              ve = visuals.end();

    while ( v != ve )
    {

      x11Visual = gkg::X11Visual::getImplementation( *v );
      if ( x11Visual->getXVisual() == x11VisualInfo.xVisual )
      {

        return *v;

      }
      ++ v;

    }

  }

  visual = new gkg::X11Visual( x11VisualInfo );
  display->getImplementation()->addVisual( visual );

  return visual;

}


gkg::X11Visual* gkg::X11Visual::getImplementation( gkg::Visual* visual )
{

  return static_cast< X11Visual* >( visual );

}



void gkg::X11Visual::setShift( uint32_t mask,
                               uint32_t& v,
                               uint32_t& shift )
{

  shift = 0;
  v = mask;
  while ( ( v & 0x1 ) == 0 )
  {

    ++ shift;
    v >>= 1;

  }

}


void gkg::X11Visual::findVisualByClassName( const std::string& name,
                                            gkg::X11VisualInfo& x11VisualInfo )
{

  for ( gkg::VisualTable* v = &visualClasses[ 0 ]; v->className; v++ )
  {

    if ( name == v->className )
    {

      XVisualInfo xVisualInfo;
      xVisualInfo.c_class = v->classTag;
      findVisualByInfo( xVisualInfo, VisualClassMask, x11VisualInfo );
      return;

    }
 
  }

}


bool gkg::X11Visual::findLayer( const std::string& name, int32_t& layer )
{

  if ( gkg::StringConverter::toScalar( name, layer ) )
  {

    return true;

  }
  if ( gkg::StringConverter::toLower( name ) == "true" ||
       gkg::StringConverter::toLower( name ) == "yes" )
  {

    layer = 0;
    return true;

  }
  return false;

}


// Look for an overlay visual.  There is no X standard way to do this,
// so we rely on the convention that the SERVER_OVERLAY_VISUALS property
// on the root window contains a list of overlay visuals.  Each visual
// has 4 pieces of information: the visual id, the type of transparency,
// the pixel or mask for transparency, and the overlay layer.  Layers
// are numbered from top-to-bottom.
void gkg::X11Visual::findOverlay( int32_t layer,
                                  gkg::X11VisualInfo& x11VisualInfo )
{

  XDisplay* xDisplay = x11VisualInfo.xDisplay;
  Atom actualType;
  int32_t actualFormat;
  unsigned long nItems; // Do not modify since it is architecture dependent
  unsigned long bytesAfter;

  gkg::X11OverlayInfo* overlayVisuals = 0;
  gkg::X11OverlayInfo** overlayVisualsPtr = &overlayVisuals;

  if ( XGetWindowProperty( xDisplay,
                           RootWindow( xDisplay, x11VisualInfo.screen ),
                           XInternAtom( xDisplay, "SERVER_OVERLAY_VISUALS",
                                        False ),
                           INT64_C( 0 ),       // offset
                           ( int64_t )1000000, // length
                           False,              // delete
                           AnyPropertyType,
                           &actualType,
                           &actualFormat,
                           &nItems,
                           &bytesAfter,
                           ( unsigned char** )overlayVisualsPtr ) != Success )
    return;

  if ( ( actualType != None ) && ( actualFormat == 32 ) && ( nItems >= 4 ) )
  {

    uint32_t nOverlays = nItems >> 2;
    for ( uint32_t i = 0; i < nOverlays; i++ )
    {

      if ( ( layer == 0 ) || ( overlayVisuals[ i ].layer == layer ) )
      {

        // we must query the visual info to find out the depth of the overlay
        XVisualInfo xVisualInfo;
        xVisualInfo.visualid = overlayVisuals[ i ].id;
        findVisualByInfo( xVisualInfo, VisualIDMask, x11VisualInfo );
        x11VisualInfo.overlay = overlayVisuals[ i ];
        break;

      }

    }

  }
  if ( overlayVisuals )
  {

    XFree( ( char* )overlayVisuals );

  }

}


// Look for a visual matching the given xinfo.
void gkg::X11Visual::findVisualByInfo( XVisualInfo& xVisualInfo, int32_t mask,
                                       gkg::X11VisualInfo& x11VisualInfo )
{

  xVisualInfo.screen = x11VisualInfo.screen;
  int32_t nVisuals = 0;
  XVisualInfo* visuals = XGetVisualInfo( x11VisualInfo.xDisplay,
                                         VisualScreenMask | mask,
                                         &xVisualInfo, &nVisuals );
  if ( visuals )
  {

    if ( nVisuals > 0 )
    {

      x11VisualInfo.depth = visuals[ 0 ].depth;
      x11VisualInfo.xVisual = visuals[ 0 ].visual;

    }
    XFree( ( char* )visuals );

  }

}


// Compute a reasonable pixel for xor'ing.  Note that this should be done
// after the visual is selected to handle the DirectColor case correctly.
uint32_t gkg::X11Visual::MSB( uint32_t i )
{

  return ( i ^ ( i >> 1 ) ) & i;

}


double gkg::X11Visual::distance( uint16_t r,
                                 uint16_t g,
                                 uint16_t b,
                                 const XColor& xColor )
{

  double scale = 1.0 / double( 0x10000 );
  double rr = ( ( double )r -  ( double )xColor.red ) * scale;
  double gg = ( ( double )g -  ( double )xColor.green ) * scale;
  double bb = ( ( double )b -  ( double )xColor.blue ) * scale;
  return rr * rr + gg * gg + bb * bb;

}


uint32_t gkg::X11Visual::rescale( uint32_t value,
                                  uint32_t inScale,
                                  uint32_t outScale )
{

  return ( value * outScale + inScale / 2 ) / inScale;

}
