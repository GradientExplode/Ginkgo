#include <gkg-graphics-X11-core/InputHandler.h>
#include <gkg-graphics-X11-core/InputHandlerImplementation.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/EventHandler.h>
#include <gkg-graphics-X11-core/Transformer.h>
#include <gkg-graphics-X11-core/GeometryAllocationInfo.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Hit.h>


gkg::InputHandler::InputHandler( gkg::Glyph* glyph, gkg::Style* style )
                  : gkg::MonoGlyph( glyph )
{

  _inputHandlerImplementation = new gkg::InputHandlerImplementation( this,
                                                                     style );
  if ( _inputHandlerImplementation )
  {

    _inputHandlerImplementation->addReference();

  }

}


gkg::InputHandler::~InputHandler()
{

  gkg::GeometryAllocationInfo* 
    info = _inputHandlerImplementation->getMostRecentInfo();
  if ( info )
  {

    info->getCanvas()->getWindow()->getDisplay()->ungrab(
                                            _inputHandlerImplementation, true );

  }
  if ( _inputHandlerImplementation )
  {

    _inputHandlerImplementation->removeReference();

  }

}


gkg::EventHandler* gkg::InputHandler::getEventHandler() const
{

  return _inputHandlerImplementation;

}


gkg::InputHandler* gkg::InputHandler::getParent() const
{

  return _inputHandlerImplementation->getParent();

}


gkg::Style* gkg::InputHandler::getStyle() const
{

  return _inputHandlerImplementation->getStyle();

}


void gkg::InputHandler::appendInputHandler( gkg::InputHandler* inputHandler )
{

  _inputHandlerImplementation->appendInputHandler( inputHandler );

}


void gkg::InputHandler::removeInputHandler( gkg::Glyph::Index index )
{

  _inputHandlerImplementation->removeInputHandler( index );

}


void gkg::InputHandler::removeAllInputHandlers()
{

  _inputHandlerImplementation->removeAllInputHandlers();

}


gkg::Glyph::Index gkg::InputHandler::getInputHandlerCount() const
{

  return _inputHandlerImplementation->getInputHandlerCount();

}


gkg::InputHandler* 
gkg::InputHandler::getInputHandler( gkg::Glyph::Index index ) const
{

  return _inputHandlerImplementation->getInputHandler( index );

}


void gkg::InputHandler::focus( gkg::InputHandler* inputHandler )
{

  _inputHandlerImplementation->focus( inputHandler );

}


void gkg::InputHandler::nextFocus()
{

  _inputHandlerImplementation->nextFocus();

}


void gkg::InputHandler::previousFocus()
{

  _inputHandlerImplementation->previousFocus();

}


void gkg::InputHandler::allocate(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  gkg::GeometryAllocationInfo&
    info = _inputHandlerImplementation->getInfo( canvas, geometryAllocation );
  geometryExtension.merge( info.getGeometryExtension() );
  geometryAllocationChanged( canvas, geometryAllocation );

}


void gkg::InputHandler::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::GeometryAllocationInfo&
    info = _inputHandlerImplementation->getInfo( canvas, geometryAllocation );
  gkg::Glyph* glyph = getBody();
  if ( glyph && canvas->isDamaged( info.getGeometryExtension() ) )
  {

    glyph->draw( canvas, geometryAllocation );

  }

}


void gkg::InputHandler::pick( gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              int32_t depth,
                              gkg::Hit& hit )
{

  gkg::GeometryAllocationInfo&
    info = _inputHandlerImplementation->getInfo( canvas, geometryAllocation );
  const gkg::Event* event = hit.getEvent();
  gkg::Event::Type type = ( event  ? event->getType() : gkg::Event::Undefined );
  switch ( type )
  {

    case gkg::Event::Key:

      if ( _inputHandlerImplementation->isInside( *event, info ) )
      {

        gkg::InputHandler* 
          focusHandler = _inputHandlerImplementation->getFocusHandler();
        gkg::InputHandlerImplementation* 
          handler = ( focusHandler ?  focusHandler->getImplementation() :
                                      _inputHandlerImplementation );
        hit.setTarget( depth, this, 0, handler ); 
        this->gkg::MonoGlyph::pick( canvas, geometryAllocation,
                                    depth + 1, hit );

      }
      break;

    case gkg::Event::Undefined:
    case gkg::Event::OtherEvent:

      this->gkg::MonoGlyph::pick( canvas, geometryAllocation, depth, hit );
      break;

    default:

      hit.begin( depth, this, 0, _inputHandlerImplementation );
      this->gkg::MonoGlyph::pick( canvas, geometryAllocation, depth + 1, hit );
      hit.end();
      break;

  }


}


void gkg::InputHandler::undraw()
{

  this->gkg::MonoGlyph::undraw();
  _inputHandlerImplementation->undraw();

}


void gkg::InputHandler::move( const gkg::Event& )
{
}


void gkg::InputHandler::press( const gkg::Event& )
{
}


void gkg::InputHandler::drag( const gkg::Event& )
{
}


void gkg::InputHandler::release( const gkg::Event& )
{
}


void gkg::InputHandler::keyStroke( const gkg::Event& event )
{

  if ( _inputHandlerImplementation->getFocusItem() != -1 )
  {

    _inputHandlerImplementation->getFocusHandler()->keyStroke( event );

  }

}


void gkg::InputHandler::doubleClick( const gkg::Event& )
{
}


gkg::InputHandler* gkg::InputHandler::focusIn()
{

  return this;

}


void gkg::InputHandler::focusOut()
{
}


void gkg::InputHandler::geometryAllocationChanged(
                                            Canvas*, const GeometryAllocation& )
{
}


bool gkg::InputHandler::isInside( const Event& event )
{

  gkg::GeometryAllocationInfo*
    info = _inputHandlerImplementation->getMostRecentInfo();
  return info && _inputHandlerImplementation->isInside( event, *info );

}


gkg::Canvas* gkg::InputHandler::getCanvas() const
{

  gkg::GeometryAllocationInfo*
    info = _inputHandlerImplementation->getMostRecentInfo();
  return info ? info->getCanvas() : 0;

}


const gkg::Transformer& gkg::InputHandler::getTransformer() const
{

  return _inputHandlerImplementation->getMostRecentInfo()->getTransformer();

}


const gkg::GeometryAllocation& gkg::InputHandler::getGeometryAllocation() const
{

  return 
      _inputHandlerImplementation->getMostRecentInfo()->getGeometryAllocation();

}


void gkg::InputHandler::redraw() const
{

  gkg::GeometryAllocationInfo*
    info = _inputHandlerImplementation->getMostRecentInfo();
  if ( info )
  {

    info->getCanvas()->setDamage( info->getGeometryExtension() );

  }

}


void gkg::InputHandler::repick( int32_t depth, gkg::Hit& hit )
{

  gkg::Canvas* canvas = getCanvas();
  if ( canvas )
  {

    const gkg::Transformer& transformer = getTransformer();
    canvas->pushTransform();
    canvas->setTransformer( transformer );
    hit.pushTransform();
    hit.setTransform( transformer );
    pick( canvas, getGeometryAllocation(), depth, hit );
    hit.popTransform();
    canvas->popTransform();

  }

}


gkg::InputHandlerImplementation* gkg::InputHandler::getImplementation() const
{

  return _inputHandlerImplementation;

}
