#include <gkg-graphics-X11-core/Printer.h>
#include <gkg-graphics-X11-core/PrinterImplementation.h>
#include <gkg-graphics-X11-core/PSPrinterImplementation.h>


gkg::Printer::Printer( std::ostream* out, const std::string& type )
{

  if ( type == "postscript" )
  {

    _printerImplementation = new gkg::PSPrinterImplementation( this, out );

  }

}


gkg::Printer::~Printer()
{

  flush();
  delete _printerImplementation;

}


gkg::PixelCoordinate gkg::Printer::toPixelCoordinate(
                                    gkg::FloatCoordinate floatCoordinate ) const
{

  return _printerImplementation->toPixelCoordinate( floatCoordinate );

}


gkg::FloatCoordinate gkg::Printer::toFloatCoordinate(
                                    gkg::PixelCoordinate pixelCoordinate ) const
{

  return _printerImplementation->toFloatCoordinate( pixelCoordinate );

}


gkg::FloatCoordinate gkg::Printer::toPixelThenFloatCoordinate(
                                    gkg::FloatCoordinate floatCoordinate ) const
{

  return _printerImplementation->toPixelThenFloatCoordinate( floatCoordinate );

}


void gkg::Printer::resize( gkg::FloatCoordinate left,
                           gkg::FloatCoordinate bottom,
                           gkg::FloatCoordinate right,
                           gkg::FloatCoordinate top )
{

  _printerImplementation->resize( left, bottom, right, top );
  flush();
  setSize( right - left, top - bottom );
  setDamage( left, bottom, right, top );

}


void gkg::Printer::prolog( const std::string& creator )
{

  _printerImplementation->prolog( creator );

}


void gkg::Printer::epilog()
{

  flush();
  _printerImplementation->epilog();

}


void gkg::Printer::setComment( const std::string& comment )
{

  flush();
  _printerImplementation->setComment( comment );

}


void gkg::Printer::setPage( const std::string& label )
{

  flush();
  _printerImplementation->setPage( label );

}


void gkg::Printer::newPath()
{

  flush();
  _printerImplementation->newPath();

}


void gkg::Printer::moveTo( gkg::FloatCoordinate x, gkg::FloatCoordinate y )
{

  flush();
  _printerImplementation->moveTo( x, y );

}


void gkg::Printer::lineTo( gkg::FloatCoordinate x, gkg::FloatCoordinate y )
{

  flush();
  _printerImplementation->lineTo( x, y );

}


void gkg::Printer::curveTo( gkg::FloatCoordinate x, gkg::FloatCoordinate y,
                            gkg::FloatCoordinate x1, gkg::FloatCoordinate y1,
                            gkg::FloatCoordinate x2, gkg::FloatCoordinate y2 )
{

  flush();
  _printerImplementation->curveTo( x, y, x1, y1, x2, y2 );

}


void gkg::Printer::closePath()
{

  flush();
  _printerImplementation->closePath();

}


void gkg::Printer::stroke( const gkg::Color* color,
                           const gkg::Brush* brush )
{

  flush();
  _printerImplementation->stroke( color, brush );

}


void gkg::Printer::fill( const gkg::Color* color )
{

  flush();
  _printerImplementation->fill( color );

}


void gkg::Printer::character( const gkg::Font* font, int32_t ch,
                              gkg::FloatCoordinate width,
                              const gkg::Color* color,
                              gkg::FloatCoordinate x, gkg::FloatCoordinate y )
{

  _printerImplementation->character( font, ch, width, color, x, y );

}


void gkg::Printer::stencil( const gkg::Bitmap* bitmap,
                            const gkg::Color* color,
                            gkg::FloatCoordinate x, gkg::FloatCoordinate y )
{

  flush();
  _printerImplementation->stencil( bitmap, color, x, y );

}


void gkg::Printer::image( const gkg::Raster* raster,
                          gkg::FloatCoordinate x, gkg::FloatCoordinate y )
{

  flush();
  _printerImplementation->image( raster, x, y );

}


void gkg::Printer::pushTransform()
{

  gkg::Canvas::pushTransform();
  flush();
  _printerImplementation->pushTransform();

}


void gkg::Printer::transform( const gkg::Transformer& transformer )
{

  gkg::Canvas::transform( transformer );
  flush();
  _printerImplementation->transform( transformer );

}


void gkg::Printer::popTransform()
{

  flush();
  _printerImplementation->pushTransform();
  gkg::Canvas::popTransform();

}


void gkg::Printer::pushClipping()
{

  flush();
  _printerImplementation->pushClipping();

}


void gkg::Printer::clip()
{

  flush();
  _printerImplementation->clip();

}


void gkg::Printer::popClipping()
{

  flush();
  _printerImplementation->popClipping();

}


void gkg::Printer::flush()
{

  _printerImplementation->flush();

}


gkg::PrinterImplementation* gkg::Printer::getImplementation() const
{

  return _printerImplementation;

}
