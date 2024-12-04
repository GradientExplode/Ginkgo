#include <gkg-graphics-X11-core/WindowImplementation.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/Glyph.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/CanvasImplementation.h>
#include <gkg-graphics-X11-core/Cursor.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/EventHandler.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/DisplayImplementation.h>
#include <gkg-graphics-X11-core/Session.h>


gkg::WindowImplementation::WindowImplementation( gkg::Window* window,
                                                 gkg::Glyph* glyph )
                          : _target( window ),
                            _glyph( glyph ),
                            _style( 0 ),
                            _display( 0 ),
                            _visual( 0 ),
                            _canvas( new gkg::Canvas( window ) ),
                            _cursor( gkg::Cursor::getDefault() ),
                            _left( 0 ),
                            _bottom( 0 ),
                            _focusIn( 0 ),
                            _focusOut( 0 ),
                            _wmDelete( 0 ),
                            _xPosition( 0 ),
                            _yPosition( 0 ),
                            _topLevelWindow( window ),
                            _placed( false ),
                            _aligned( false ),
                            _needsResize( false ),
                            _resized( false ),
                            _moved( false ),
                            _unmapped( false ),
                            _wmMapped( false ),
                            _mapPending( false )
{

  if ( _glyph )
  {

    _glyph->addReference();

  }

}


gkg::WindowImplementation::~WindowImplementation()
{

  if ( _glyph )
  {

    _glyph->removeReferenceDeferred();

  }
  if ( _style )
  {

    _style->removeReferenceDeferred();

  }
  if ( _focusIn )
  {

    _focusIn->removeReferenceDeferred();

  }
  if ( _focusOut )
  {

    _focusOut->removeReferenceDeferred();

  }
  if ( _wmDelete )
  {

    _wmDelete->removeReferenceDeferred();

  }
  delete _canvas;
  _cursors.clear();

}


gkg::Window* gkg::WindowImplementation::getTarget() const
{

  return _target;

}


gkg::Glyph* gkg::WindowImplementation::getGlyph() const
{

  return _glyph;

}


void gkg::WindowImplementation::setStyle( gkg::Style* style )
{

  if ( _style != style )
  {

    if ( style )
    {

      style->addReference();

    }
    if ( _style )
    {

      _style->removeReference();

    }
    _style = style;
    checkBinding();

  }

}


gkg::Style* gkg::WindowImplementation::getStyle() const
{

  return _style;

}


void gkg::WindowImplementation::setDisplay( gkg::Display* display )
{

  if ( _display != display )
  {

    checkBinding();
    _display = display;
    _canvas->getImplementation()->setDisplay( display );

  }

}


gkg::Display* gkg::WindowImplementation::getDisplay() const
{

  return _display;

}


// this method should only be used for unvisible window(s)
void gkg::WindowImplementation::forceDisplay( Display* display )
{

  _display = display;

}


gkg::Visual* gkg::WindowImplementation::getVisual() const
{

  return _visual;

}


gkg::Canvas* gkg::WindowImplementation::getCanvas() const
{

  return _canvas;

}


gkg::Cursor* gkg::WindowImplementation::getCursor() const
{

  return _cursor;

}


void gkg::WindowImplementation::pushCursor()
{

  _cursors.push_back( _cursor );

}


void gkg::WindowImplementation::popCursor()
{

  if ( !_cursors.empty() )
  {

    setCursor( _cursors.back() );
    _cursors.pop_back();

  }

}


void gkg::WindowImplementation::setPlace( gkg::FloatCoordinate left,
                                          gkg::FloatCoordinate bottom )
{

  if ( !_placed ||
       !gkg::equal( left, _left, ( float )1e-3 ) ||
       !gkg::equal( bottom, _bottom, ( float )1e-3 ) )
  {

    checkBinding();
    _placed = true;
    _left = left;
    _bottom = bottom;

  }

}


void gkg::WindowImplementation::setPixelPlace( gkg::IntegerCoordinate left,
                                               gkg::IntegerCoordinate bottom )
{

  checkBinding();
  _placed = true;
  _left = _display->toFloatCoordinate( left );
  _bottom = _display->toFloatCoordinate( bottom );

}


void gkg::WindowImplementation::align( float x, float y )
{

  if ( !_aligned ||
       !gkg::equal( x, _xAlign, ( float )1e-3 ) ||
       !gkg::equal( y, _yAlign, ( float )1e-3 ) )
  {

    checkBinding();
    _aligned = true;
    _xAlign = x;
    _yAlign = y;

  }

}


gkg::FloatCoordinate gkg::WindowImplementation::getLeft()
{

  if ( !_display )
  {

    return _left;

  }
  checkPosition();
  return _display->toFloatCoordinate( _xPosition );

}


gkg::FloatCoordinate gkg::WindowImplementation::getBottom()
{

  if ( !_display )
  {

    return _bottom;

  }
  checkPosition();
  return _display->getHeight() -
         _display->toFloatCoordinate( _yPosition ) -
         _canvas->getHeight();

}


gkg::FloatCoordinate gkg::WindowImplementation::getWidth() const
{

  return _canvas->getWidth();

}


gkg::FloatCoordinate gkg::WindowImplementation::getHeight() const
{

  return _canvas->getHeight();

}


void gkg::WindowImplementation::map()
{

  if ( _mapPending || _wmMapped )
  {

    return;

  }
  _unmapped = false;
  if ( isBound() )
  {

    _display->getImplementation()->addWindow( _target );

  }
  else
  {

    _target->unbind();
    if ( !_display )
    {

      setDisplay( gkg::Session::getInstance().getDefaultDisplay() );

    }
    if ( !_style )
    {

      setStyle( new gkg::Style( _display->getStyle() ) );

    }
    _target->configure();
    _target->setDefaultGeometry();
    _target->computeGeometry();
    _target->bind();
    _target->setProperties();

  }
  _target->doMap();

}


void gkg::WindowImplementation::unmap()
{

  if ( _mapPending || _wmMapped )
  {

    _glyph->undraw();
    doUnmap();
    _display->getImplementation()->removeWindow( _target );
    _canvas->getImplementation()->clearDamage();
    _unmapped = true;
    _wmMapped = false;

  }

}


bool gkg::WindowImplementation::isMapped() const
{

  return _wmMapped;

}


void gkg::WindowImplementation::repair()
{

  if ( _canvas->getImplementation()->startRepair() )
  {

    _glyph->draw( _canvas, _geometryAllocation );
    _canvas->getImplementation()->finishRepair();

  }

}


// note that a window has moved.
void gkg::WindowImplementation::moveInPixelCoordinate( int32_t x, int32_t y )
{

  _xPosition = x;
  _yPosition = y;
  _moved = false;

}


// resize a window, allocating the associated glyph and damaging the new area
void gkg::WindowImplementation::resizeInPixelCoordinate(
                                                   gkg::PixelCoordinate width,
                                                   gkg::PixelCoordinate height )
{

  _canvas->setPixelSize( width, height );
  _canvas->setDamageAll();
  const gkg::GeometryRequirement&
    rx = _geometryShape.getRequirement( gkg::DimensionX );
  const gkg::GeometryRequirement&
    ry = _geometryShape.getRequirement( gkg::DimensionY );
  gkg::FloatCoordinate xSize = _canvas->getWidth();
  gkg::FloatCoordinate ySize = _canvas->getHeight();
  gkg::FloatCoordinate xOrigin = xSize * rx.getAlignment();
  gkg::FloatCoordinate yOrigin = ySize * ry.getAlignment();
  _geometryAllocation.setAllotment( gkg::DimensionX,
                    gkg::GeometryAllotment( xOrigin, xSize, xOrigin / xSize ) );
  _geometryAllocation.setAllotment( gkg::DimensionY,
                    gkg::GeometryAllotment( yOrigin, ySize, yOrigin / ySize ) );
  gkg::GeometryExtension geometryExtension;
  geometryExtension.clear();
  initRenderer();
  if ( _resized )
  {

    _glyph->undraw();

  }
  _glyph->allocate( _canvas, _geometryAllocation, geometryExtension );
  _resized = true;

}


void gkg::WindowImplementation::checkBinding()
{

  if ( _unmapped )
  {

    _target->unbind();

  }

}


void gkg::WindowImplementation::initRenderer()
{

  _canvas->getImplementation()->unbind();
  _canvas->getImplementation()->bind( _style->getValueIsOn("double_buffered") );

}


void gkg::WindowImplementation::setFocusEvent( gkg::EventHandler* in,
                                               gkg::EventHandler* out )
{

  if ( in )
  {

    in->addReference();

  }
  if ( out )
  {

    out->addReference();

  }
  if ( _focusIn )
  {

    _focusIn->removeReference();

  }
  if ( _focusOut )
  {

    _focusOut->removeReference();

  }
  _focusIn = in;
  _focusOut = out;

}


void 
gkg::WindowImplementation::setWmDelete( gkg::EventHandler* eventHandler )
{

  if ( eventHandler )
  {

    eventHandler->addReference();

  }
  if ( _wmDelete )
  {

    _wmDelete->removeReference();

  }
  _wmDelete = eventHandler;

}


void gkg::WindowImplementation::configure()
{
}


void gkg::WindowImplementation::setDefaultGeometry()
{

  _glyph->request( _geometryShape );
  gkg::FloatCoordinate
    width = _geometryShape.getRequirement( gkg::DimensionX ).getNatural();
  gkg::FloatCoordinate
    height = _geometryShape.getRequirement( gkg::DimensionY ).getNatural();
  _canvas->setSize( width, height );
  _xPosition = _display->toPixelCoordinate( _left );
  _yPosition = _display->getPixelHeight() -
               _display->toPixelCoordinate( _bottom ) -
               _canvas->getPixelHeight();
  if ( _aligned )
  {

    _xPosition -= _display->toPixelCoordinate( _xAlign * width );
    _yPosition += _display->toPixelCoordinate( _yAlign * height );

  }

}

void gkg::WindowImplementation::computeGeometry()
{
}


void gkg::WindowImplementation::setProperties()
{
}


void gkg::WindowImplementation::setPlaced( bool value )
{

  _placed = value;

}


bool gkg::WindowImplementation::isPlaced() const
{

  return _placed;

}


void gkg::WindowImplementation::setXPosition( int xPosition )
{

  _xPosition = xPosition;

}


void gkg::WindowImplementation::setYPosition( int yPosition )
{

  _yPosition = yPosition;

}


int gkg::WindowImplementation::getXPosition() const
{

  return _xPosition;

}


int gkg::WindowImplementation::getYPosition() const
{

  return _yPosition;

}


gkg::Window* gkg::WindowImplementation::getTopLevelWindow() const
{

  return _topLevelWindow;

}


const gkg::GeometryRequisition& 
gkg::WindowImplementation::getGeometryShape() const
{

  return _geometryShape;

}


