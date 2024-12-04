#include <gkg-graphics-X11-core/InputHandlerImplementation.h>
#include <gkg-graphics-X11-core/InputHandler.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/GeometryAllocationInfo.h>
#include <gkg-graphics-X11-core/GeometryAllocTable.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Hit.h>


uint32_t gkg::InputHandlerImplementation::_threshold = 0;


gkg::InputHandlerImplementation::InputHandlerImplementation(
                                                gkg::InputHandler* inputHandler,
                                                gkg::Style* style )
                                : _target( inputHandler ),
                                  _style( style ),
                                  _focusItem( -1 ),
                                  _focusHandler( 0 ),
                                  _parent( 0 ),
                                  _geometryAllocations( 0 )
{

  if ( _style )
  {

    _style->addReference();

  }
  reset();
  if ( _threshold == 0 )
  {

    int32_t threshold = INT32_C( 250 );
    _style->findAttribute( "clickDelay", threshold );
    _threshold = threshold;

  }

}


gkg::InputHandlerImplementation::~InputHandlerImplementation()
{

  if ( _style )
  {

    _style->removeReference();

  }
  delete _geometryAllocations;

}


gkg::InputHandler* gkg::InputHandlerImplementation::getTarget() const
{

  return _target;

}


void gkg::InputHandlerImplementation::setParent( gkg::InputHandler* parent )
{

  _parent = parent;

}


gkg::InputHandler* gkg::InputHandlerImplementation::getParent() const
{

  return _parent;

}


gkg::Style* gkg::InputHandlerImplementation::getStyle() const
{

  return _style;

}


void gkg::InputHandlerImplementation::appendInputHandler( 
                                               gkg::InputHandler* inputHandler )
{

  if ( inputHandler )
  {

    _children.push_back( inputHandler );
    inputHandler->getImplementation()->setParent( _target );

  }

}


void 
gkg::InputHandlerImplementation::removeInputHandler( gkg::Glyph::Index index )
{

  if ( _children[ index ] == _focusHandler )
  {

    nextFocus();
    if ( _children[ index ] == _focusHandler )
    {

      _focusHandler = 0;

    }

  }
  _children.erase( _children.begin() + index );

}


void gkg::InputHandlerImplementation::removeAllInputHandlers()
{

  _children.clear();
  _focusHandler = 0;

}


gkg::Glyph::Index gkg::InputHandlerImplementation::getInputHandlerCount() const
{

  return ( gkg::Glyph::Index )_children.size();

}


gkg::InputHandler* gkg::InputHandlerImplementation::getInputHandler(
                                                 gkg::Glyph::Index index ) const
{

  return _children[ index ];

}


void gkg::InputHandlerImplementation::focus( gkg::InputHandler* inputHandler )
{

  gkg::Glyph::Index count = ( gkg::Glyph::Index )_children.size();
  for ( gkg::Glyph::Index g = 0; g < count; g++ )
  {

    if ( _children[ g ] == inputHandler )
    {

      if ( _focusHandler )
      {

        _focusHandler->focusOut();

      }
      _focusItem = g;
      _focusHandler = inputHandler->focusIn();
      break;

    }

  }

}


void gkg::InputHandlerImplementation::nextFocus()
{

  if ( _focusHandler )
  {

    _focusHandler->focusOut();

  }
  gkg::Glyph::Index count = ( gkg::Glyph::Index )_children.size();
  gkg::Glyph::Index f = _focusItem + 1;
  if ( f >= count )
  {

    if ( _parent )
    {

      _parent->nextFocus();
      return;

    }
    else if ( count == 0 )
    {

      return;

    }
    else
    {

      f = 0;

    }

  }
  _focusItem = f;
  _focusHandler = _children[ f ]->focusIn();

}


void gkg::InputHandlerImplementation::previousFocus()
{

  if ( _focusHandler )
  {

    _focusHandler->focusOut();

  }
  gkg::Glyph::Index count = ( gkg::Glyph::Index )_children.size();
  gkg::Glyph::Index f = _focusItem - 1;
  if ( f < 0 )
  {

    if ( _parent )
    {

      _parent->previousFocus();
      return;

    }
    else if ( count == 0 )
    {

      return;

    }
    else
    {

      f = count - 1;

    }

  }
  _focusItem = f;
  _focusHandler = _children[ f ]->focusIn();

}


gkg::Glyph::Index gkg::InputHandlerImplementation::getFocusItem() const
{

  return _focusItem;

}


gkg::InputHandler* 
gkg::InputHandlerImplementation::getFocusHandler() const
{

  return _focusHandler;

}


void gkg::InputHandlerImplementation::undraw()
{

  if ( _geometryAllocations )
  {

    gkg::GeometryAllocationInfo* info = getMostRecentInfo();
    if ( info )
    {

      gkg::Window* window = info->getCanvas()->getWindow();
      if ( window )
      {

        window->getDisplay()->ungrab( this, true );

      }

    }
    _geometryAllocations->flush();

  }

}


bool gkg::InputHandlerImplementation::event( gkg::Event& event )
{

  bool handled = true;
  switch ( event.getType() )
  {

    case gkg::Event::Down:

      down( event );
      break;

    case gkg::Event::Motion:

      motion( event );
      break;

    case gkg::Event::Up:

      up( event );
      break;

    case gkg::Event::Key:

      _target->keyStroke( event );
      break;

    default:

      // ignore
      break;

  }
  return handled;

}


gkg::GeometryAllocationInfo& 
gkg::InputHandlerImplementation::getInfo( 
                             gkg::Canvas* canvas,
                             const gkg::GeometryAllocation& geometryAllocation )
{

  if ( !_geometryAllocations )
  {

    _geometryAllocations = new gkg::GeometryAllocTable( 0, 1 );

  }

  gkg::GeometryAllocationInfo*
    info = _geometryAllocations->find( canvas, geometryAllocation );
  if ( !info )
  {

    // The need for this code is unfortunate. The problem is that an input 
    // handler needs to ensure that it ungrabs if a canvas/window is replaced
    // from the allocation table.  Perhaps there should be a general-purpose
    // interface for this from allocation table, but for now we know InputHandler
    // only keeps a single allocation. So, before allocating a new one we check 
    // to see if there is an old one that has a valid window.  Then we do the
    // ungrab. If we didn't do anything about the ungrab, then handler might 
    // stay grabbed even when we forgot about the window.
    gkg::GeometryAllocationInfo*
      oldInfo = _geometryAllocations->getMostRecent();
    if ( oldInfo )
    {

      gkg::Canvas* oldCanvas = oldInfo->getCanvas();
      if ( oldCanvas )
      {

        gkg::Window* oldWindow = oldCanvas->getWindow();
        if ( oldWindow && ( oldWindow != canvas->getWindow() ) )
        {

          oldWindow->getDisplay()->ungrab( this, true );

        }

      }

    }
    info = _geometryAllocations->allocate( canvas, geometryAllocation );
    gkg::GeometryExtension geometryExtension;
    geometryExtension.clear();
    _target->MonoGlyph::allocate( canvas,
                                  geometryAllocation,
                                  geometryExtension );
    info->setGeometryExtension( geometryExtension );

  }
  return *info;

}


gkg::GeometryAllocationInfo* 
gkg::InputHandlerImplementation::getMostRecentInfo()
{

  if ( _geometryAllocations )
  {

    gkg::GeometryAllocationInfo*
      info = _geometryAllocations->getMostRecent();
    if ( info && info->getCanvas() )
    {

      return info;

    }

  }
  return 0;

}


void gkg::InputHandlerImplementation::reset()
{

  _pressed = false;
  _recordedTime = false;

}


void gkg::InputHandlerImplementation::down( gkg::Event& event )
{

  if ( !_pressed )
  {

    _pressed = true;
    _button = event.getPointerButton();
    _target->press( event );
    event.grab( this );
    if ( _parent )
    {

      _parent->focus( _target );

    }
    else
    {

      if ( _focusHandler != _target )
      {

        if ( _focusHandler )
        {

         _focusHandler->focusOut();
         _focusItem = -1;

        }
        _focusHandler = _target->focusIn();

      }

    }

  }

}


void gkg::InputHandlerImplementation::motion( gkg::Event& event )
{

  if ( _pressed )
  {

    _target->drag( event );

  }
  else
  {

    _target->move( event );

  }

}


void gkg::InputHandlerImplementation::up( gkg::Event& event )
{

  if ( _pressed && event.getPointerButton() == _button )
  {

    _pressed = false;
    event.ungrab( this );
    _target->release( event );
    uint32_t time = event.getTime();
    if ( _recordedTime && ( time - _clickTime < _threshold ) )
    {

      _target->doubleClick( event );

    }
    _clickTime = time;
    _recordedTime = true;

  }

}


bool gkg::InputHandlerImplementation::isInside( 
                                       const gkg::Event& event,
                                       const gkg::GeometryAllocationInfo& info )
{

  gkg::FloatCoordinate x = event.getPointerX();
  gkg::FloatCoordinate y = event.getPointerY();
  gkg::Canvas* canvas = info.getCanvas();
  gkg::Window* window = canvas->getWindow();
  if ( !window || ( window != event.getWindow() ) )
  {

    return false;

  }
  const gkg::GeometryExtension& 
    geometryExtension = info.getGeometryExtension();
  if ( ( x < geometryExtension.getRight() ) &&
       ( x >= geometryExtension.getLeft() ) &&
       ( y < geometryExtension.getTop() ) &&
       ( y >= geometryExtension.getBottom() ) )
  {

    const gkg::Transformer& transformer = info.getTransformer();
    gkg::Hit hit( &event );
    hit.setTransform( transformer );
    canvas->pushTransform();
    canvas->setTransformer( transformer );
    _target->MonoGlyph::pick( canvas, info.getGeometryAllocation(), 0, hit );
    canvas->popTransform();
    if ( hit.hasAny() )
    {

      return true;

    }

  }
  return false;

}
