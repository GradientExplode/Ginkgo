#include <gkg-graphics-X11-core/ActiveHandler.h>
#include <gkg-graphics-X11-core/Event.h>


gkg::ActiveHandler::ActiveHandler( gkg::Glyph* glyph,
                                    gkg::Style* style )
                   : gkg::InputHandler( glyph, style ),
                     _inside( false )
{
}


gkg::ActiveHandler::~ActiveHandler()
{
}


void gkg::ActiveHandler::undraw()
{

  if ( _inside )
  {

    _inside = false;
    leave();

  }
  gkg::InputHandler::undraw();

}


void gkg::ActiveHandler::move( const gkg::Event& event )
{

  gkg::EventHandler* eventHandler = getEventHandler();
  if ( event.getEventHandler() == eventHandler )
  {

    if ( !_inside )
    {

      _inside = true;
      event.grab( eventHandler );
      enter();

    }

  }
  else if ( _inside )
  {

    _inside = false;
    leave();
    event.ungrab( eventHandler );

  }

}


void gkg::ActiveHandler::drag( const gkg::Event& event )
{

  move( event );

}


void gkg::ActiveHandler::enter()
{
}


void gkg::ActiveHandler::leave()
{
}


