#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/SessionImplementation.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/Event.h>


gkg::Session::Session()
{

  _sessionImplementation = 
    gkg::getGUIImplementationFactory()->createSessionImplementation( this );

}


gkg::Session::~Session()
{

  delete _sessionImplementation;

}


const std::string& gkg::Session::getClassName() const
{

  return _sessionImplementation->getClassName();

}


const std::string& gkg::Session::getName() const
{

  return _sessionImplementation->getName();

}


int32_t gkg::Session::getArgc() const
{

  return _sessionImplementation->getArgc();

}


char** gkg::Session::getArgv() const
{

  return _sessionImplementation->getArgv();

}


gkg::Style* gkg::Session::getStyle() const
{

  return _sessionImplementation->getStyle();

}


void gkg::Session::setDefaultDisplay( gkg::Display* display )
{

  _sessionImplementation->setDefaultDisplay( display );

}


gkg::Display* gkg::Session::getDefaultDisplay() const
{

  return _sessionImplementation->getDefaultDisplay();

}


gkg::Display* gkg::Session::connect( const std::string& displayName )
{

  gkg::Display* display = new gkg::Display( displayName );
  if ( display )
  {

    _sessionImplementation->connect( display );

  }
  return display;

}


void gkg::Session::disconnect( gkg::Display* display )
{

  _sessionImplementation->disconnect( display );


}


int32_t gkg::Session::run()
{

  return _sessionImplementation->run();

}


int32_t gkg::Session::runWindow( gkg::Window* window )
{

  window->map();
  return run();  

}


void gkg::Session::quit()
{

  _sessionImplementation->quit();

}


bool gkg::Session::isDone() const
{

  return _sessionImplementation->isDone();

}


bool gkg::Session::pending() const
{

  return _sessionImplementation->pending();

}


void gkg::Session::read( gkg::Event& event )
{

  _sessionImplementation->read( event );

}


bool gkg::Session::read( int32_t sec, int32_t usec, gkg::Event& event )
{

  return _sessionImplementation->read( sec, usec, event );

}


void gkg::Session::unread( gkg::Event& event )
{

  // put an event back from whence it came
  event.unread();

}


void gkg::Session::poll( gkg::Event& event )
{

  // poll an event; implies the event already has an associated display
  event.poll();

}


void 
gkg::Session::initialize( const std::string& className,
                          int32_t& argc, char* argv[],
                          const gkg::OptionDescription* options,
                          const gkg::PropertyData* properties )
{

  _sessionImplementation->initialize( className, argc, argv,
                                      options, properties );

}


gkg::SessionImplementation* gkg::Session::getImplementation() const
{

  return _sessionImplementation;

}


