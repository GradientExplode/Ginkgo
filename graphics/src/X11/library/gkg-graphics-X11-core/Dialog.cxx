#include <gkg-graphics-X11-core/Dialog.h>
#include <gkg-graphics-X11-core/DialogHandler.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/TransientWindow.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Session.h>


gkg::Dialog::Dialog( gkg::Glyph* glyph, gkg::Style* style )
            : gkg::InputHandler( glyph, style )
{
}


gkg::Dialog::~Dialog()
{
}


bool gkg::Dialog::postFor( gkg::Window* window )
{

  return postForAligned( window, 0.5, 0.5 );

}

bool gkg::Dialog::postForAligned( gkg::Window* window,
                                  float xAlign, float yAlign )
{

  gkg::TransientWindow* transientWindow = new gkg::TransientWindow( this );
  transientWindow->setStyle( new gkg::Style( getStyle() ) );
  transientWindow->setTransientFor( window );
  transientWindow->setWmDelete( new gkg::DialogHandler( this ) );
  transientWindow->setPlace( window->getLeft() + 0.5 * window->getWidth(),
                             window->getBottom() + 0.5 * window->getHeight() );
  transientWindow->align( xAlign, yAlign );
  transientWindow->map();
  bool b = run();
  transientWindow->unmap();
  transientWindow->getDisplay()->sync();
  delete transientWindow;
  return b;

}


bool gkg::Dialog::postAt( gkg::FloatCoordinate x, gkg::FloatCoordinate y )
{

  return postAtAligned( x, y, 0.5, 0.5 );

}


bool gkg::Dialog::postAtAligned( gkg::FloatCoordinate x, gkg::FloatCoordinate y,
                                 float xAlign, float yAlign )
{

  gkg::TransientWindow* transientWindow = new gkg::TransientWindow( this );
  transientWindow->setStyle( new gkg::Style( getStyle() ) );
  transientWindow->setWmDelete( new gkg::DialogHandler( this ) );
  transientWindow->setPlace( x, y );
  transientWindow->align( xAlign, yAlign );
  transientWindow->map();
  bool b = run();
  transientWindow->unmap();
  transientWindow->getDisplay()->sync();
  delete transientWindow;
  return b;

}


bool gkg::Dialog::run()
{

  gkg::Session& session = gkg::Session::getInstance();
  gkg::Event event;
  _done = false;
  for ( ; ; )
  {

    session.read( event );
    if ( event.getGrabber() || isInside( event ) )
    {

      event.handle();

    }
    else if ( event.getType() == gkg::Event::Key )
    {

      keyStroke( event );

    }
    if ( _done )
    {

      break;

    }
    if ( session.isDone() ) 
    {

      _accepted = false;
      break;

    }

  }
  return _accepted;

}


void gkg::Dialog::dismiss( bool accept )
{

  _accepted = accept;
  _done = true;

}
