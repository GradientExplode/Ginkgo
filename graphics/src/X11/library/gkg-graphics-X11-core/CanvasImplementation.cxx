#include <gkg-graphics-X11-core/CanvasImplementation.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Transformer.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/DisplayImplementation.h>
#include <gkg-graphics-X11-core/GeometryExtension.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/FontImplementation.h>
#include <gkg-graphics-X11-core/Bitmap.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Brush.h>


gkg::CanvasImplementation::FontCharTable 
gkg::CanvasImplementation::_charBitmaps;


gkg::CanvasImplementation::CanvasImplementation( gkg::Canvas* canvas,
                                                 gkg::Window* window )
                          : _target( canvas ),
                            _display( 0 ),
                            _window( window ),
                            _width( 0 ),
                            _height( 0 ),
                            _pixelWidth( 0 ),
                            _pixelHeight( 0 ),
                            _brush( 0 ),
                            _color( 0 ),
                            _font( 0 ),
                            _damaged( false ),
                            _onDamageList( false ),
                            _repairing( false )
{

  gkg::Transformer* identity = new gkg::Transformer;
  _transformers.push_back( identity );
  _transformed = false;

}


gkg::CanvasImplementation::~CanvasImplementation()
{

  std::list< gkg::Transformer* >::iterator t = _transformers.begin(),
                                           te = _transformers.end();
  while ( t != te )
  {

    delete *t;
    ++ t;

  }
  _transformers.clear();

}


gkg::Canvas* gkg::CanvasImplementation::getTarget() const
{

  return _target;

}


gkg::Display* gkg::CanvasImplementation::getDisplay() const
{

  return _display;

}


gkg::Window* gkg::CanvasImplementation::getWindow() const
{

  return _window;

}


void gkg::CanvasImplementation::setDisplay( gkg::Display* display )
{

  _display = display;

}


void gkg::CanvasImplementation::setWidth( gkg::FloatCoordinate width )
{

  _width = width;
  if ( _display )
  {

    _pixelWidth = _display->toPixelCoordinate( width );

  }

}


void gkg::CanvasImplementation::setHeight( gkg::FloatCoordinate height )
{

  _height = height;
  if ( _display )
  {

    _pixelHeight = _display->toPixelCoordinate( height );

  }

}


void gkg::CanvasImplementation::setSize( gkg::FloatCoordinate width,
                                         gkg::FloatCoordinate height )
{

  _width = width;
  _height = height;
  if ( _display )
  {

    _pixelWidth = _display->toPixelCoordinate( width );
    _pixelHeight = _display->toPixelCoordinate( height );

  }

}


gkg::FloatCoordinate gkg::CanvasImplementation::getWidth() const
{

  return _width;

}


gkg::FloatCoordinate gkg::CanvasImplementation::getHeight() const
{

  return _height;

}


void 
gkg::CanvasImplementation::setPixelWidth( gkg::PixelCoordinate pixelWidth )
{

  _pixelWidth = pixelWidth;
  if ( _display )
  {

    _width = _display->toFloatCoordinate( pixelWidth );

  }

}


void 
gkg::CanvasImplementation::setPixelHeight( gkg::PixelCoordinate pixelHeight )
{

  _pixelHeight = pixelHeight;
  if ( _display )
  {

    _height = _display->toFloatCoordinate( pixelHeight );

  }

}


void gkg::CanvasImplementation::setPixelSize( gkg::PixelCoordinate pixelWidth,
                                              gkg::PixelCoordinate pixelHeight )
{

  _pixelWidth = pixelWidth;
  _pixelHeight = pixelHeight;
  if ( _display )
  {

    _width = _display->toFloatCoordinate( pixelWidth );
    _height = _display->toFloatCoordinate( pixelHeight );

  }

}


gkg::PixelCoordinate gkg::CanvasImplementation::getPixelWidth() const
{

  return _pixelWidth;

}


gkg::PixelCoordinate gkg::CanvasImplementation::getPixelHeight() const
{

  return _pixelHeight;

}


gkg::PixelCoordinate gkg::CanvasImplementation::toPixelCoordinate(
                                    gkg::FloatCoordinate floatCoordinate ) const
{

  return _display->toPixelCoordinate( floatCoordinate );

}


gkg::FloatCoordinate gkg::CanvasImplementation::toFloatCoordinate(
                                    gkg::PixelCoordinate pixelCoordinate ) const
{

  return _display->toFloatCoordinate( pixelCoordinate );

}


gkg::FloatCoordinate gkg::CanvasImplementation::toPixelThenFloatCoordinate(
                                    gkg::FloatCoordinate floatCoordinate ) const
{

  return _display->toFloatCoordinate(
                               _display->toPixelCoordinate( floatCoordinate ) );

}


void gkg::CanvasImplementation::string( const gkg::Font* font,
                                        const std::string& s,
                                        const gkg::Color* color,
                                        gkg::FloatCoordinate x,
                                        gkg::FloatCoordinate y )
{

  gkg::FloatCoordinate cumulatedWidth = 0;
  int32_t ch = 0;
  gkg::FloatCoordinate width = 0;
  for ( size_t c = 0; c < s.length(); c++ )
  {

    ch = s[ c ];
    width = font->getWidth( ch );
    character( font, ch, width, color, x + cumulatedWidth, y );
    cumulatedWidth += width;

  }

}


void gkg::CanvasImplementation::pushTransform()
{

  flush();
  _transformers.push_back( new gkg::Transformer( getTransformer() ) );

}


void gkg::CanvasImplementation::transform( const gkg::Transformer& transformer )
{

  flush();
  Transformer* t = _transformers.back();
  t->preMultiply( transformer );
  _transformed = !t->isIdentity();

}


void gkg::CanvasImplementation::popTransform()
{

  flush();

  // we pushed the first matrix during initialization,
  // so we must be underflowing the stack.  Should be an exception.
  if ( _transformers.empty() )
  {

    return;

  }
  Transformer* transformer = _transformers.back();
  delete transformer;
  _transformers.pop_back();
  _transformed = !getTransformer().isIdentity();

}


void gkg::CanvasImplementation::setTransformer(
                                           const gkg::Transformer& transformer )
{

  flush();
  Transformer* t = _transformers.back();
  *t = transformer;
  _transformed = !transformer.isIdentity();

}


const gkg::Transformer& gkg::CanvasImplementation::getTransformer() const
{

  Transformer* transformer = _transformers.back();
  return *transformer;

}


void gkg::CanvasImplementation::setDamage(
                                       const gkg::GeometryExtension& extension )
{

  setDamage( extension.getLeft(), extension.getBottom(),
             extension.getRight(), extension.getTop() );

}


void gkg::CanvasImplementation::setDamage( gkg::FloatCoordinate left,
                                           gkg::FloatCoordinate bottom,
                                           gkg::FloatCoordinate right,
                                           gkg::FloatCoordinate top )
{

  if ( _damaged )
  {

    _damage.left = std::min( _damage.left, left );
    _damage.bottom = std::min( _damage.bottom, bottom );
    _damage.right = std::max( _damage.right, right );
    _damage.top = std::max( _damage.top, top );

  }
  else
  {

    _damage.left = left;
    _damage.bottom = bottom;
    _damage.right = right;
    _damage.top = top;

  }
  newDamage();

}


void gkg::CanvasImplementation::setDamageAll()
{

  setDamage( 0, 0, _width, _height );

}


bool gkg::CanvasImplementation::isDamaged(
                                 const gkg::GeometryExtension& extension ) const
{

  return isDamaged( extension.getLeft(),
                    extension.getBottom(),
                    extension.getRight(),
                    extension.getTop() );

}


bool gkg::CanvasImplementation::isDamaged( gkg::FloatCoordinate left,
                                           gkg::FloatCoordinate bottom,
                                           gkg::FloatCoordinate right,
                                           gkg::FloatCoordinate top ) const
{

  return _damaged &&
         ( left < _damage.right ) &&
         ( right > _damage.left ) &&
         ( bottom < _damage.top ) &&
         ( top > _damage.bottom );

}


bool gkg::CanvasImplementation::hasAnyDamage() const
{

  return _damaged;

}


void gkg::CanvasImplementation::getDamageArea(
                                       gkg::GeometryExtension& extension ) const
{

  extension.setXY( 0,_damage.left, _damage.bottom, _damage.right, _damage.top );

}


void gkg::CanvasImplementation::restrictDamage(
                                       const gkg::GeometryExtension& extension )
{

  restrictDamage( extension.getLeft(),
                  extension.getBottom(),
                  extension.getRight(),
                  extension.getTop() );

}


void gkg::CanvasImplementation::restrictDamage( gkg::FloatCoordinate left,
                                                gkg::FloatCoordinate bottom,
                                                gkg::FloatCoordinate right,
                                                gkg::FloatCoordinate top )
{

  clearDamage();
  setDamage( left, bottom, right, top );
  if ( _repairing )
  {

    startRepair();

  }

}


void gkg::CanvasImplementation::newDamage()
{

  _damaged = true;
  if ( !_onDamageList && _window && _window->isBound() )
  {

    _onDamageList = true;
    _display->getImplementation()->needsRepair( _window );

  }

}


void gkg::CanvasImplementation::clearDamage()
{

  _damaged = false;
  _onDamageList = false;

}


void gkg::CanvasImplementation::repair()
{

  clearDamage();

}


void gkg::CanvasImplementation::finishRepair()
{

  flush();
  swapBuffers();
  clearDamage();
  _repairing = false;

}


void gkg::CanvasImplementation::referenceColor( const gkg::Color* color )
{

  if ( color )
  {

    color->addReference();

  }
  if ( _color )
  {

    _color->removeReference();

  }
  _color = color;

}


void gkg::CanvasImplementation::referenceBrush( const gkg::Brush* brush )
{

  if ( brush )
  {

    brush->addReference();

  }
  if ( _brush )
  {

    _brush->removeReference();

  }
  _brush = brush;

}


void gkg::CanvasImplementation::referenceFont( const gkg::Font* font )
{

  if ( font )
  {

    font->addReference();

  }
  if ( _font )
  {

    _font->removeReference();

  }
  _font = font;

}


void gkg::CanvasImplementation::unreferenceAll()
{

  if ( _brush )
  {

    _brush->removeReference();

  }
  if ( _color )
  {

    _color->removeReference();

  }
  if ( _font )
  {

    _font->removeReference();

  }
  _brush = 0;
  _color = 0;
  _font = 0;

}


bool gkg::CanvasImplementation::isRectangular( int32_t x0, int32_t y0,
                                               int32_t x1, int32_t y1,
                                               int32_t x2, int32_t y2,
                                               int32_t x3, int32_t y3 )
{

  return ( x0 == x1 && y1 == y2 && x2 == x3 && y3 == y0 ) ||
         ( x0 == x3 && y3 == y2 && x2 == x1 && y1 == y0 );

}


const gkg::Bitmap* 
gkg::CanvasImplementation::getCharBitmap( gkg::Display* display,
                                          const gkg::Font* font,
                                          int32_t ch )
{

  const gkg::Bitmap* bitmap = 0;

  gkg::FontCharEntry entry( font, ch );
  gkg::CanvasImplementation::FontCharTable::const_iterator 
    b = _charBitmaps.find( entry );
  if ( b != _charBitmaps.end() )
  {

    bitmap = ( *b ).second;

  }
  else
  {

    bitmap = new gkg::Bitmap( entry.font, entry.ch,
                              font->getImplementation( display )->getScale() );
    _charBitmaps[ entry ] = bitmap;

  }

  return bitmap;

}
