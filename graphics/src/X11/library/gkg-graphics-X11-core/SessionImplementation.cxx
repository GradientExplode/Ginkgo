#include <gkg-graphics-X11-core/SessionImplementation.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Cursor.h>
#include <gkg-communication-dispatcher/Dispatcher.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

//
// SessionImplementation
//

gkg::SessionImplementation::SessionImplementation(
                                  gkg::Session* session,
                                  const gkg::PropertyData* defaultProperties,
                                  const gkg::OptionDescription* defaultOptions )
                           : _target( session ),
                             _argc( 0 ),
                             _argv( 0 ),
                             _done( false ),
                             _readInput( true ),
                             _style( 0 ),
                             _properties( 0 ),
                             _defaultDisplay( 0 ),
                             _defaultProperties( defaultProperties ),
                             _defaultOptions( defaultOptions )
{
}


gkg::SessionImplementation::~SessionImplementation()
{

  if ( _style )
  {

    _style->removeReference();

  }
  std::list< gkg::Display* >::iterator d = _displays.begin(),
                                       de = _displays.end();
  while ( d != de )
  {

    delete *d;
    ++ d;

  }
  _displays.clear();
  delete [] _argv;

}


const std::string& gkg::SessionImplementation::getClassName() const
{

  return _className;

}


const std::string& gkg::SessionImplementation::getName() const
{

  return _name;

}


int32_t gkg::SessionImplementation::getArgc() const
{

  return _argc;

}


char** gkg::SessionImplementation::getArgv() const
{

  return _argv;

}


gkg::Style* gkg::SessionImplementation::getStyle() const
{

  if ( _defaultDisplay )
  {

    return _defaultDisplay->getStyle();

  }
  return _style;

}


void gkg::SessionImplementation::setDefaultDisplay( gkg::Display* display )
{

  _defaultDisplay = display;

}


gkg::Display* gkg::SessionImplementation::getDefaultDisplay() const
{

  return _defaultDisplay;

}


void gkg::SessionImplementation::connect( gkg::Display* display )
{

  setStyle( display );
  gkg::Dispatcher::getInstance().link( 
                                   display->getFd(),
                                   gkg::Dispatcher::ReadMask,
                                   new gkg::SessionIOHandler( this, display ) );
  _displays.push_back( display );

}


void gkg::SessionImplementation::disconnect( gkg::Display* display )
{

  std::list< gkg::Display* >::iterator d = _displays.begin(),
                                       de = _displays.end();
  while ( d != de )
  {

    if ( *d == display )
    {

      gkg::Dispatcher::getInstance().unlink( display->getFd() );
      display->close();
      _displays.erase( d );
      break;

    }
    ++ d;

  }

}


// main session loop : read events and handle them
int32_t gkg::SessionImplementation::run()
{

  gkg::Event event;
  _done = false;
  do
  {

    read( event );
    event.handle();

  }
  while ( !_done );
  return 0;

}


// exit the main session loop
void gkg::SessionImplementation::quit()
{

  _done = true;

}


// return loop status
bool gkg::SessionImplementation::isDone() const
{

  return _done;

}


// check if an event is pending on any display
bool gkg::SessionImplementation::pending() const
{

  gkg::Event event;
  bool b = check( event );
  if ( b )
  {

    event.unread();

  }
  return b;

}


// read an event; could be from any display; the redundant-looking test of _done
// is necessary because check might change the value of done.
void gkg::SessionImplementation::read( gkg::Event& event )
{

  bool save = _readInput;
  _readInput = false;
  while ( !_done && !check( event ) )
  {

    gkg::Dispatcher::getInstance().dispatch();

  }
  _readInput = save;

}


// read an event as above, but time out after a given (sec, usec) delay;
// return true if an event was read, false if the time-out expired.
bool gkg::SessionImplementation::read( int64_t sec, int64_t usec, 
                                       gkg::Event& event )
{

  int64_t secLeft = sec;
  int64_t usecLeft = usec;
  bool save = _readInput;
  _readInput = false;
  while ( !_done && !check( event ) )
  {

    if ( !( secLeft > 0 || usecLeft > 0 ) )
    {

      _readInput = save;
      return false;

    }
    gkg::Dispatcher::getInstance().dispatch( secLeft, usecLeft );

  }
  _readInput = save;
  return true;

}


void gkg::SessionImplementation::handleDisplayInput( gkg::Display* display )
{

  if ( display->isClosed() )
  {

    _done = true;
    return;

  }
  if ( _readInput )
  {

    gkg::Event event;
    for ( ; ; )
    {

      if ( !display->getEvent( event ) )
      {

        break;

      }
      event.handle();

    }

  }

}


void 
gkg::SessionImplementation::initialize( const std::string& className,
                                        int32_t& argc, char* argv[],
                                        const gkg::OptionDescription* options,
                                        const gkg::PropertyData* properties )
{

  _argc = argc;
  _argv = new char*[ argc + 1 ];
  for ( int32_t i = 0; i < argc; i++ )
  {

    _argv[ i ] = argv[ i ];

  }
  _argv[ _argc ] = 0;

  initializeStyle( className, properties );
  if ( options )
  {

    parseArgs( argc, argv, options );

  }
  parseArgs( argc, argv, _defaultOptions );
  initializeDisplay();
  gkg::Cursor::initialize();

}


// parse the argument list, setting any properties that are specified by the
// option list; matching arguments are removed (in-place) from the argument list
void gkg::SessionImplementation::parseArgs(
                                         int32_t& argc, char** argv,
                                         const gkg::OptionDescription* options )
{

  int32_t i;
  int32_t newArgc = 1;
  char* newArgv[ 1024 ];
  newArgv[ 0 ] = argv[ 0 ];
  for ( i = 1; i < argc; i++ )
  {

    bool matched = false;
    std::string arg( argv[ i ] );
    const gkg::OptionDescription* o = 0;
    for ( o = &options[ 0 ]; o->name; o++ )
    {

      if ( match( arg, *o, i, argc, argv ) )
      {

        matched = true;
        break;

      }

    }
    if ( !matched )
    {

      newArgv[ newArgc ] = argv[ i ];
      newArgc ++;

    }

  }
  if ( newArgc < argc )
  {

    for ( i = 1; i < newArgc; i++ )
    {

      argv[ i ] = newArgv[ i ];

    }
    argc = newArgc;
    argv[ argc ] = 0;

  }

}


// see if the given argument matches the option description
bool gkg::SessionImplementation::match( const std::string& arg,
                                        const gkg::OptionDescription& option,
                                        int32_t& i, int32_t argc, char** argv )
{

  std::string opt( option.name );
  if ( arg != opt )
  {

    if ( option.style == gkg::OptionDescription::OptionValueAfter )
    {

      int32_t n = opt.length();
      if ( opt == arg.substr( 0, n ) )
      {

        _style->addAttribute( option.path, arg.substr( n, arg.length() - n ) );
        return true;

      }

    }
    return false;

  }
  std::string name, value;
  extract( arg, option, i, argc, argv, name, value );
  _style->addAttribute( name, value );
  return true;

}


// extract an attribute <name, value> from a given argument.
void 
gkg::SessionImplementation::extract( const std::string& arg,
                                     const gkg::OptionDescription& option,
                                     int32_t& i, int32_t argc, char** argv,
                                     std::string& name, std::string& value )
{

  size_t colon;
  switch ( option.style )
  {

    case gkg::OptionDescription::OptionPropertyNext:

      value = nextArg( i, argc, argv, "missing property after '%s'", arg );
      colon = value.find( ':' );
      if ( colon == std::string::npos )
      {

        badArg( "missing ':' in '%s'", value );

      }
      else
      {

        name = value.substr( 0, colon );
        value = value.substr( colon + 1, value.length() - colon - 1 );

      }
      break;

    case gkg::OptionDescription::OptionValueNext:

      name = std::string( option.path );
      value = nextArg( i, argc, argv, "missing value after '%s'", arg );
      break;

    case gkg::OptionDescription::OptionValueImplicit:

      name = std::string( option.path );
      value = std::string( option.value );
      break;

    case gkg::OptionDescription::OptionValueIsArg:

      name = std::string( option.path );
      value = arg;
      break;

    case gkg::OptionDescription::OptionValueAfter:

      badArg( "missing value in '%s'", arg );
      break;

    case gkg::OptionDescription::OptionNone:

      break;

  }

}


// report that an argument is bad and exit; a caller of this function
// may assume that it does not return.
void gkg::SessionImplementation::badArg( const char* format,
                                         const std::string& arg )
{

  fflush( stdout );
  fprintf( stderr, format, arg.c_str() );
  putc( '\n', stderr );
  exit( 1 );

}


// make sure there is another argument--if not generate an error
std::string 
gkg::SessionImplementation::nextArg( int32_t& i, int32_t argc, char** argv,
                                     const char* message,
                                     const std::string& arg )
{

  i ++;
  if ( i == argc )
  {

    badArg( message, arg );

  }
  return std::string( argv[ i ] );

}


// find the value for a specific argument
bool gkg::SessionImplementation::findArg( const std::string& name,
                                          std::string& value )
{

  int32_t last = _argc - 1;
  for ( int32_t i = 1; i < last; i++ )
  {

    if ( name == _argv[ i ] )
    {

      value = std::string( _argv[ i + 1 ] );
      return true;

    }

  }
  return false;

}


// initialize style information for the session
void 
gkg::SessionImplementation::initializeStyle( const std::string& className,
                                             const PropertyData* properties )
{

  _className = className;
  _name = findName();
  _style = new gkg::Style( _name );
  if ( _style )
  {

    _style->addReference();

  }
  _style->setAlias( _className );
  _properties = properties;

}


// use ICCCM rules to find an application's instance name from the command line
// or an environment variable
std::string gkg::SessionImplementation::findName()
{

  std::string name;
  if ( findArg( "-name", name ) )
  {

    return name;

  }

  const char* resourceName = getenv( "RESOURCE_NAME" );
  if ( resourceName )
  {

    return std::string( resourceName );

  }

  if ( _argc > 0 )
  {

    std::string s( _argv[ 0 ] );
    size_t slash = s.rfind( '/' );
    if ( slash != std::string::npos )
    {

      s = s.substr( slash + 1, s.length() - slash - 1 );

    }
    return s;

  }
  return "noname";

}


void 
gkg::SessionImplementation::loadProperties( gkg::Style* style,
                                            const gkg::PropertyData* properties,
                                            int32_t priority )
{

  if ( properties )
  {

    for ( const gkg::PropertyData* p = &properties[ 0 ]; p->path; p++ )
    {

      style->addAttribute( p->path, p->value, priority );

    }

  }

}


// open the default display and initialize its style information
void gkg::SessionImplementation::initializeDisplay()
{

  std::string name;
  if ( _style->findAttribute( "display", name ) )
  {

    _defaultDisplay = new gkg::Display( name );

  }
  else
  {

    _defaultDisplay = new gkg::Display();

  }

  if ( !_defaultDisplay )
  {

    if ( !name.empty() )
    {

      std::cerr << "can't open display " << name << std::endl;

    }
    else
    {

      std::cerr << "can't open DISPLAY" << std::endl;

    }
    ::exit( 1 );

  }
  connect( _defaultDisplay );

}


// set the style information for the given display.
// first, copy the current style (just the parsed command-line arguments),
// then add the environment style, display defaults, app defaults,
// session properties, and default properties.
void gkg::SessionImplementation::setStyle( gkg::Display* display )
{

  gkg::Style* style = new gkg::Style( *_style );

  loadProperties( style, _defaultProperties, -5 );
  loadGkgDefaults( style, -5 );
  loadProperties( style, _properties, -5 );
  loadApplicationDefaults( style, -5 );
  loadDisplayDefaults( style, display, -5 );
  loadEnvironment( style, -5 );

  display->setStyle( style );

}


// check for a pending event, returning it if there is one
bool gkg::SessionImplementation::check( Event& event ) const
{


  std::list< gkg::Display* >::const_iterator d = _displays.begin(),
                                             de = _displays.end();
  while ( d != de )
  {

    gkg::Display* display = *d;
    if ( display->getEvent( event ) )
    {

      return true;

    }
    ++ d;

  }
  return false;

}


//
// SessionIOHandler
//

gkg::SessionIOHandler::SessionIOHandler(
                              gkg::SessionImplementation* sessionImplementation,
                              gkg::Display* display )
                      : _sessionImplementation( sessionImplementation ),
                        _display( display )
{
}


gkg::SessionIOHandler::~SessionIOHandler()
{
}


int32_t gkg::SessionIOHandler::isInputReady( int32_t )
{

  _sessionImplementation->handleDisplayInput( _display );
  return 0;

}
