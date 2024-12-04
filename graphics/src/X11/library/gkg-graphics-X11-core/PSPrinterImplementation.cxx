#include <gkg-graphics-X11-core/PSPrinterImplementation.h>
#include <gkg-graphics-X11-core/Printer.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-graphics-X11-core/Bitmap.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/Raster.h>
#include <gkg-graphics-X11-core/Brush.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Transformer.h>
#include <gkg-core-cppext/OStream.h>


static const float PAGE_WIDTH = 8.5 * 72;
static const float PAGE_HEIGHT = 11 * 72;
static const float epsilon = 0.01;


static const char* ps_prolog = "\
save 20 dict begin\n\
\n\
/sf {   % scale /fontName => -  (set current font)\n\
    {findfont} stopped {pop /Courier findfont} if\n\
    exch scalefont setfont\n\
} def\n\
\n\
/ws {\n\
    4 index 6 4 roll moveto sub\n\
    2 index stringwidth pop sub\n\
    exch div 0 8#40 4 3 roll\n\
    widthshow\n\
} def\n\
\n\
/as {\n\
    4 index 6 4 roll moveto sub\n\
    2 index stringwidth pop sub\n\
    exch div 0 3 2 roll\n\
    ashow\n\
} def\n\
\n\
";

static const char* ps_epilog = "\
end restore\n\
";


gkg::PSPrinterImplementation::PSPrinterImplementation( gkg::Printer* printer,
                                                       std::ostream* out )
                             : gkg::PrinterImplementation( printer, out )
{

  gkg::PrinterImplementation::Info info;
  _infos.push_back( info );

}


gkg::PSPrinterImplementation::~PSPrinterImplementation()
{
}


gkg::PixelCoordinate gkg::PSPrinterImplementation::toPixelCoordinate(
                                    gkg::FloatCoordinate floatCoordinate ) const
{

  return ( gkg::PixelCoordinate )floatCoordinate;

}


gkg::FloatCoordinate gkg::PSPrinterImplementation::toFloatCoordinate(
                                    gkg::PixelCoordinate pixelCoordinate ) const
{

  return ( gkg::FloatCoordinate )pixelCoordinate;

}


gkg::FloatCoordinate gkg::PSPrinterImplementation::toPixelThenFloatCoordinate(
                                    gkg::FloatCoordinate floatCoordinate ) const
{

  return floatCoordinate;

}


void gkg::PSPrinterImplementation::resize( gkg::FloatCoordinate left,
                                           gkg::FloatCoordinate bottom,
                                           gkg::FloatCoordinate right,
                                           gkg::FloatCoordinate top )
{

  _x = ( ( float )( left + right ) - PAGE_WIDTH ) / 2;
  _y = ( ( float )( top + bottom ) - PAGE_HEIGHT ) / 2;

}


void gkg::PSPrinterImplementation::prolog( const std::string& creator )
{

  *_out << "%!PS-Adobe-2.0\n";
  *_out << "%%Creator: " << creator << "\n";
  *_out << "%%Pages: atend\n";
  *_out << "%%EndComments\n";
  *_out << ps_prolog;
  *_out << "%%EndProlog\n";

}


void gkg::PSPrinterImplementation::epilog()
{

  *_out << "showpage\n";
  *_out << "%%Trailer\n";
  *_out << ps_epilog;
  *_out << "%%Pages: " << _page - 1 << "\n";

}


void gkg::PSPrinterImplementation::setComment( const std::string& comment )
{

  *_out << "%% " << comment << "\n";

}


void gkg::PSPrinterImplementation::setPage( const std::string& label )
{

  if ( _page > 1 )
  {

    *_out << "showpage\n";

  }
  *_out << "%%Page: " << label << " " << _page << "\n";
  *_out << -_x << " " << -_y << " translate\n";
  gkg::PrinterImplementation::Info& info = _infos.back();
  info.font = 0;
  info.color = 0;
  info.brush = 0;
  ++ _page;

}


void gkg::PSPrinterImplementation::newPath()
{

  *_out << "newpath\n";

}


void gkg::PSPrinterImplementation::moveTo( gkg::FloatCoordinate x,
                                           gkg::FloatCoordinate y )
{

  *_out << x << " " << y << " moveto\n";

}


void gkg::PSPrinterImplementation::lineTo( gkg::FloatCoordinate x,
                                           gkg::FloatCoordinate y )
{

  *_out << x << " " << y << " lineto\n";

}


void gkg::PSPrinterImplementation::curveTo( gkg::FloatCoordinate x,
                                            gkg::FloatCoordinate y,
                                            gkg::FloatCoordinate x1,
                                            gkg::FloatCoordinate y1,
                                            gkg::FloatCoordinate x2,
                                            gkg::FloatCoordinate y2 )
{

  *_out << x1 << " " << y1 << " " << x2 << " " << y2 << " ";
  *_out << x << " " << y << " curveto\n";

}


void gkg::PSPrinterImplementation::closePath()
{

  *_out << "closepath\n";

}


void gkg::PSPrinterImplementation::stroke( const gkg::Color* color,
                                           const gkg::Brush* brush )
{

  gkg::PrinterImplementation::Info& info = _infos.back();
  if ( info.color != color )
  {

    doColor( color );
    info.color = color;

  }
  if ( info.brush != brush )
  {

    doBrush( brush );
    info.brush = brush;

  }
  *_out << "gsave stroke grestore\n";

}


void gkg::PSPrinterImplementation::fill( const gkg::Color* color )
{

  gkg::PrinterImplementation::Info& info = _infos.back();
  if ( info.color != color )
  {

   doColor( color );
   info.color = color;

  }
  *_out << "gsave eofill grestore\n";

}


void gkg::PSPrinterImplementation::character( const gkg::Font* font,
                                              int32_t ch,
                                              gkg::FloatCoordinate width,
                                              const gkg::Color* color,
                                              gkg::FloatCoordinate x,
                                              gkg::FloatCoordinate y )
{

  gkg::PrinterImplementation::Info& info = _infos.back();
  if ( info.color != color )
  {

    flush();
    doColor( color );
    info.color = color;

  }
  if ( !gkg::equal( y, _textCurrentY, epsilon ) )
  {

    flush();

  }
  if ( !gkg::equal( x, _textCurrentX, epsilon ) )
  {

    flush();

  }
  if ( info.font != font )
  {

    flush();
    doFont( font );
    info.font = font;

  }
  if ( _textChars == 0 )
  {

    *_out << x << " " << y << "(";

  }
  _textCurrentX = x + width;
  _textCurrentY = y;
  if ( ( ch == '\\' ) || ( ch == ')' ) || ( ch == '(' ) )
  {

    *_out << "\\" << ( char )ch;

  }
  else if ( ch > 127 )
  {

    *_out << "\\";

    char oldFill = 0;
    int32_t oldWidth = gkg::ostream_set_oct_mode( *_out, 3, '0', &oldFill );

    gkg::ostream_write_oct_byte( *_out, ch );

    gkg::ostream_set_dec_mode( *_out, oldWidth, oldFill, &oldFill );

  }
  else
  {

    *_out << ( char )ch;

  }
  ++ _textChars;
  if ( ch == ' ' )
  {

    ++ _textSpaces;

  }

}


void gkg::PSPrinterImplementation::stencil( const gkg::Bitmap* bitmap,
                                            const gkg::Color* color,
                                            gkg::FloatCoordinate x,
                                            gkg::FloatCoordinate y )
{

  gkg::PrinterImplementation::Info& info = _infos.back();
  if ( info.color != color )
  {

    doColor( color );
    info.color = color;

  }
  int32_t width = bitmap->getPixelWidth();
  int32_t height = bitmap->getPixelHeight();
  int32_t bytes = ( width - 1 ) / 8 + 1;
  gkg::FloatCoordinate left = x - bitmap->getLeftBearing();
  gkg::FloatCoordinate right = x + bitmap->getRightBearing();
  gkg::FloatCoordinate bottom = y - bitmap->getDescent();
  gkg::FloatCoordinate top = y + bitmap->getAscent();
  *_out << "gsave\n";
  *_out << "/picstr " << bytes << " string def\n";
  *_out << left << " " << bottom << "  translate\n";
  *_out << right - left << " " << top - bottom << " scale\n";
  *_out << width << " " << height << " true\n";
  *_out << "[" << width << " 0 0 " << height << " 0 0]\n";
  *_out << "{currentfile picstr readhexstring pop} imagemask\n";

  int32_t oldWidth = gkg::ostream_set_hex_mode( *_out, 1 );

  for ( int32_t iy = 0; iy < height; iy++ )
  {

    for ( int32_t ix = 0; ix < bytes; ix++ )
    {

      int32_t theByte = 0;
      for ( int32_t bit = 0; bit < 8; bit++ )
      {

        if ( bitmap->peek( ix * 8 + bit, iy ) )
        {

          theByte |= 0x80 >> bit;

        }

      }
      gkg::ostream_write_hex_byte( *_out, theByte );

    }
    *_out << "\n";

  }
  gkg::ostream_set_dec_mode( *_out, oldWidth );

  *_out << "grestore\n";

}


void gkg::PSPrinterImplementation::image( const gkg::Raster* raster,
                                          gkg::FloatCoordinate x,
                                          gkg::FloatCoordinate y )
{

  int32_t width = raster->getPixelWidth();
  int32_t height = raster->getPixelHeight();
  float left = ( float )x - raster->getLeftBearing();
  float right = ( float )x + raster->getRightBearing();
  float bottom = ( float )y - raster->getDescent();
  float top = ( float )y + raster->getAscent();
  *_out << "gsave\n";
  *_out << "/picstr " << width << " string def\n";
  *_out << left << " " << bottom << "  translate\n";
  *_out << right - left << " " << top - bottom << " scale\n";
  *_out << width << " " << height << " 8\n";
  *_out << "[" << width << " 0 0 " << height << " 0 0]\n";
  *_out << "{currentfile picstr readhexstring pop} image\n";

  int32_t oldWidth = gkg::ostream_set_hex_mode( *_out, 1 );

  for ( int32_t iy = 0; iy < height; iy++ )
  {

    for ( int32_t ix = 0; ix < width; ix++ )
    {

      float r, g, b, alpha;
      raster->peek( ix, iy, r, g, b, alpha );
      int32_t theByte = ( int32_t )( 0xff * ( r + g + b ) / 3 );

      gkg::ostream_write_hex_byte( *_out, theByte );

    }
    *_out << "\n";

  }
  gkg::ostream_set_dec_mode( *_out, oldWidth );

  *_out << "grestore\n";

}


void gkg::PSPrinterImplementation::pushTransform()
{

  _infos.push_back( _infos.back() );
  *_out << "gsave\n";

}


void gkg::PSPrinterImplementation::transform(
                                           const gkg::Transformer& transformer )
{

  float a00, a01, a10, a11, a20, a21;
  transformer.getMatrix( a00, a01, a10, a11, a20, a21 );
  *_out << "[" << a00 << " " << a01;
  *_out << " " << a10 << " " << a11;
  *_out << " " << a20 << " " << a21 << "] concat\n";

}


void gkg::PSPrinterImplementation::popTransform()
{

  _infos.pop_back();
  *_out << "grestore\n";

}


// the current transform push/pop just saves and restores state,
// so we can use it for clipping as well.
void gkg::PSPrinterImplementation::pushClipping()
{

  _infos.push_back( _infos.back() );
  *_out << "gsave\n";

}


void gkg::PSPrinterImplementation::clip()
{

  *_out << "eoclip\n";

}


void gkg::PSPrinterImplementation::popClipping()
{

  _infos.pop_back();
  *_out << "grestore\n";

}


void gkg::PSPrinterImplementation::flush()
{

  if ( _textChars > 0 )
  {

    *_out << ") ";
    if ( _textSpaces > 0 )
    {

      *_out << _textSpaces << " " << _textCurrentX << " ws\n";

    }
    else
    {

      *_out << _textChars << " " << _textCurrentX << " as\n";

    }
    _textChars = 0;
    _textSpaces = 0;

  }

}


gkg::PSPrinterImplementation* 
gkg::PSPrinterImplementation::getImplementation( gkg::Printer* printer )
{

  return static_cast< PSPrinterImplementation* >( printer->getImplementation() );

}


void gkg::PSPrinterImplementation::doColor( const gkg::Color* color )
{

  float r, g, b;
  color->getIntensities( r, g, b );
  *_out << r << " " << g << " " << b << " setrgbcolor\n";

}


void gkg::PSPrinterImplementation::doBrush( const gkg::Brush* brush )
{

  gkg::FloatCoordinate lineWidth = brush->getWidth();
  *_out << lineWidth << " setlinewidth\n";

  // Should do something about patterned brushes.
  // Maybe something like this:
  //
  //  int dcnt = brush->getDashCount();
  //  if ( dcnt != 0 )
  //  {
  //   *_out << "[";
  //   for ( int i = 0; i < dcnt; i++ )
  //     *_out << " " << brush->getDashList( i );
  //   *_out << " ] 0 setdash\n";
  //  }

}


void gkg::PSPrinterImplementation::doFont( const gkg::Font* font )
{

  *_out << font->getSize() << " /";
  const std::string& name = font->getName();
  size_t length = name.length();
  size_t p = 0;
  while ( p < length )
  {

    *_out << ( name[ p ] == ' ' ? '-' : name[ p ] );
    ++ p;

  }
  *_out << " sf\n";

}




