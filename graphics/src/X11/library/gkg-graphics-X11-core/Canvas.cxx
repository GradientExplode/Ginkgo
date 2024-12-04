#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/CanvasImplementation.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


gkg::Canvas::Canvas( gkg::Window* window )
{

  _canvasImplementation = 
    gkg::getGUIImplementationFactory()->createCanvasImplementation( this,
                                                                    window );

}


gkg::Canvas::~Canvas()
{

  delete _canvasImplementation;

}


gkg::Window* gkg::Canvas::getWindow() const
{

  return _canvasImplementation->getWindow();

}


void gkg::Canvas::setWidth( gkg::FloatCoordinate width )
{

  _canvasImplementation->setWidth( width );

}


void gkg::Canvas::setHeight( gkg::FloatCoordinate height )
{

  _canvasImplementation->setHeight( height );

}


void gkg::Canvas::setSize( gkg::FloatCoordinate width,
                           gkg::FloatCoordinate height )
{

  _canvasImplementation->setSize( width, height );

}


gkg::FloatCoordinate gkg::Canvas::getWidth() const
{

  return _canvasImplementation->getWidth();

}


gkg::FloatCoordinate gkg::Canvas::getHeight() const
{

  return _canvasImplementation->getHeight();

}


void gkg::Canvas::setPixelWidth( gkg::PixelCoordinate pixelWidth )
{

  _canvasImplementation->setPixelWidth( pixelWidth );

}


void gkg::Canvas::setPixelHeight( gkg::PixelCoordinate pixelHeight )
{

  _canvasImplementation->setPixelHeight( pixelHeight );

}


void gkg::Canvas::setPixelSize( gkg::PixelCoordinate width,
                                gkg::PixelCoordinate height )
{

  _canvasImplementation->setPixelSize( width, height );

}


gkg::PixelCoordinate gkg::Canvas::getPixelWidth() const
{

  return _canvasImplementation->getPixelWidth();

}


gkg::PixelCoordinate gkg::Canvas::getPixelHeight() const
{

  return _canvasImplementation->getPixelHeight();

}


gkg::PixelCoordinate 
gkg::Canvas::toPixelCoordinate( gkg::FloatCoordinate floatCoordinate ) const
{

  return _canvasImplementation->toPixelCoordinate( floatCoordinate );

}


gkg::FloatCoordinate
gkg::Canvas::toFloatCoordinate( gkg::PixelCoordinate pixelCoordinate ) const
{

  return _canvasImplementation->toFloatCoordinate( pixelCoordinate );

}


gkg::FloatCoordinate 
gkg::Canvas::toPixelThenFloatCoordinate( gkg::FloatCoordinate floatCoordinate ) const
{

  return _canvasImplementation->toPixelThenFloatCoordinate( floatCoordinate );

}


void gkg::Canvas::newPath()
{

  _canvasImplementation->newPath();

}


void gkg::Canvas::moveTo( gkg::FloatCoordinate x, gkg::FloatCoordinate y )
{

  _canvasImplementation->moveTo( x, y );

}


void gkg::Canvas::lineTo( gkg::FloatCoordinate x, gkg::FloatCoordinate y )
{

  _canvasImplementation->lineTo( x, y );

}


void gkg::Canvas::curveTo( gkg::FloatCoordinate x, gkg::FloatCoordinate y,
                           gkg::FloatCoordinate x1, gkg::FloatCoordinate y1,
                           gkg::FloatCoordinate x2, gkg::FloatCoordinate y2 )
{

  _canvasImplementation->curveTo( x, y, x1, y1, x2, y2 );

}


void gkg::Canvas::closePath()
{

  _canvasImplementation->closePath();

}


void gkg::Canvas::stroke( const gkg::Color* color,
                          const gkg::Brush* brush )
{

  _canvasImplementation->stroke( color, brush );

}


void gkg::Canvas::line( gkg::FloatCoordinate x1, gkg::FloatCoordinate y1,
                        gkg::FloatCoordinate x2, gkg::FloatCoordinate y2,
                        const gkg::Color* color,
                        const gkg::Brush* brush )
{

  newPath();
  moveTo( x1, y1 );
  lineTo( x2, y2 );
  stroke( color, brush );

}


void gkg::Canvas::rectangle( gkg::FloatCoordinate left,
                             gkg::FloatCoordinate bottom,
                             gkg::FloatCoordinate right,
                             gkg::FloatCoordinate top,
                             const gkg::Color* color,
                             const gkg::Brush* brush )
{

  newPath();
  moveTo( left, bottom );
  lineTo( left, top );
  lineTo( right, top );
  lineTo( right, bottom );
  closePath();
  stroke( color, brush );

}


void gkg::Canvas::fill( const gkg::Color* color )
{

  _canvasImplementation->fill( color );

}


void gkg::Canvas::fillRectangle( gkg::FloatCoordinate left,
                                 gkg::FloatCoordinate bottom,
                                 gkg::FloatCoordinate right,
                                 gkg::FloatCoordinate top,
                                 const gkg::Color* color )
{

  newPath();
  moveTo( left, bottom );
  lineTo( left, top );
  lineTo( right, top );
  lineTo( right, bottom );
  closePath();
  fill( color );

}


void gkg::Canvas::character( const gkg::Font* font, int32_t ch,
                             gkg::FloatCoordinate width,
                             const gkg::Color* color,
                             gkg::FloatCoordinate x, gkg::FloatCoordinate y )
{

  _canvasImplementation->character( font, ch, width, color, x, y );

}


void gkg::Canvas::string( const gkg::Font* font, const std::string& s,
                          const gkg::Color* color,
                          gkg::FloatCoordinate x, gkg::FloatCoordinate y )
{

  _canvasImplementation->string( font, s, color, x, y );

}


void gkg::Canvas::stencil( const gkg::Bitmap* bitmap,
                           const gkg::Color* color,
                           gkg::FloatCoordinate x, gkg::FloatCoordinate y )
{

  _canvasImplementation->stencil( bitmap, color, x, y );

}


void gkg::Canvas::image( const gkg::Raster* raster,
                         gkg::FloatCoordinate x, gkg::FloatCoordinate y )
{

  _canvasImplementation->image( raster, x, y );

}


void gkg::Canvas::pushTransform()
{

  _canvasImplementation->pushTransform();

}


void gkg::Canvas::transform( const gkg::Transformer& transformer )
{

  _canvasImplementation->transform( transformer );

}


void gkg::Canvas::popTransform()
{

  _canvasImplementation->popTransform();

}


void gkg::Canvas::setTransformer( const gkg::Transformer& transformer )
{

  _canvasImplementation->setTransformer( transformer );

}


const gkg::Transformer& gkg::Canvas::getTransformer() const
{

  return _canvasImplementation->getTransformer();

}


void gkg::Canvas::pushClipping()
{

  _canvasImplementation->pushClipping();

}


void gkg::Canvas::clip()
{

  _canvasImplementation->clip();

}


void gkg::Canvas::clipRectangle( gkg::FloatCoordinate left,
                                 gkg::FloatCoordinate bottom,
                                 gkg::FloatCoordinate right,
                                 gkg::FloatCoordinate top )
{

  newPath();
  moveTo( left, bottom );
  lineTo( left, top );
  lineTo( right, top );
  lineTo( right, bottom );
  closePath();
  clip();

}


void gkg::Canvas::popClipping()
{

  _canvasImplementation->popClipping();

}


void gkg::Canvas::frontBuffer()
{

  _canvasImplementation->frontBuffer();

}


void gkg::Canvas::backBuffer()
{

  _canvasImplementation->backBuffer();

}


void gkg::Canvas::setDamage( const gkg::GeometryExtension& extension )
{

  _canvasImplementation->setDamage( extension );

}


void gkg::Canvas::setDamage( gkg::FloatCoordinate left,
                             gkg::FloatCoordinate bottom,
                             gkg::FloatCoordinate right,
                             gkg::FloatCoordinate top )
{

  _canvasImplementation->setDamage( left, bottom, right, top );

}


void gkg::Canvas::setDamageAll()
{

  _canvasImplementation->setDamageAll();

}


bool 
gkg::Canvas::isDamaged( const gkg::GeometryExtension& extension ) const
{

  return _canvasImplementation->isDamaged( extension );

}


bool gkg::Canvas::isDamaged( gkg::FloatCoordinate left,
                             gkg::FloatCoordinate bottom,
                             gkg::FloatCoordinate right,
                             gkg::FloatCoordinate top ) const
{

  return _canvasImplementation->isDamaged( left, bottom, right, top );

}


bool gkg::Canvas::hasAnyDamage() const
{

  return _canvasImplementation->hasAnyDamage();

}


void gkg::Canvas::getDamageArea( gkg::GeometryExtension& extension ) const
{

  _canvasImplementation->getDamageArea( extension );

}


void 
gkg::Canvas::restrictDamage( const gkg::GeometryExtension& extension )
{

  _canvasImplementation->restrictDamage( extension );

}


void gkg::Canvas::restrictDamage( gkg::FloatCoordinate left,
                                  gkg::FloatCoordinate bottom,
                                  gkg::FloatCoordinate right,
                                  gkg::FloatCoordinate top )
{

  _canvasImplementation->restrictDamage( left, bottom, right, top );

}


void gkg::Canvas::redraw( gkg::FloatCoordinate left,
                          gkg::FloatCoordinate bottom,
                          gkg::FloatCoordinate right,
                          gkg::FloatCoordinate top )
{

  _canvasImplementation->redraw( left, bottom, right, top );

}


void gkg::Canvas::repair()
{

  _canvasImplementation->repair();

}


gkg::CanvasImplementation* gkg::Canvas::getImplementation() const
{

  return _canvasImplementation;

}


