#include <gkg-graphics-X11-X11/X11SessionImplementation.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-communication-tcpip/Host.h>
#include <cstdlib>


static gkg::PropertyData defaultProperties[] =
{

  { "*background", "#ffffff" },
  { "*foreground", "#000000" },
  { "*font", "fixed" },
  { 0, 0 }

};


static gkg::OptionDescription defaultOptions[] =
{

  { "-background", "*background",
    gkg::OptionDescription::OptionValueNext, 0 },
  { "-bg", "*background",
    gkg::OptionDescription::OptionValueNext, 0 },
  { "-dbuf", "*double_buffered",
    gkg::OptionDescription::OptionValueImplicit, "on" },
  { "-display", "*display",
    gkg::OptionDescription::OptionValueNext, 0 },
  { "-dpi", "*dpi",
    gkg::OptionDescription::OptionValueNext, 0 },
  { "-fg", "*foreground",
    gkg::OptionDescription::OptionValueNext, 0 },
  { "-flat", "*flat",
    gkg::OptionDescription::OptionValueNext, 0 },
  { "-fn", "*font",
    gkg::OptionDescription::OptionValueNext, 0 },
  { "-font", "*font",
    gkg::OptionDescription::OptionValueNext, 0 },
  { "-foreground", "*foreground",
    gkg::OptionDescription::OptionValueNext, 0 },
  { "-geometry", "*geometry",
    gkg::OptionDescription::OptionValueNext, 0 },
  { "-iconic", "*iconic",
    gkg::OptionDescription::OptionValueImplicit, "on" },
  { "-motif", "*look_and_feel",
    gkg::OptionDescription::OptionValueImplicit, "Motif" },
  { "-monochrome", "*monochrome",
    gkg::OptionDescription::OptionValueImplicit, "off" },
  { "-name", "*name",
    gkg::OptionDescription::OptionValueNext, 0 },
  { "-nodbuf", "*double_buffered",
    gkg::OptionDescription::OptionValueImplicit, "off" },
  { "-noshape", "*shaped_windows",
    gkg::OptionDescription::OptionValueImplicit, "off" },
  { "-reverse", "*reverseVideo",
    gkg::OptionDescription::OptionValueImplicit, "on" },
  { "-rv", "*reverseVideo",
    gkg::OptionDescription::OptionValueImplicit, "on" },
  { "-shape", "*shaped_windows",
    gkg::OptionDescription::OptionValueImplicit, "on" },
  { "-synchronous", "*synchronous",
    gkg::OptionDescription::OptionValueImplicit, "on" },
  { "+synchronous", "*synchronous",
    gkg::OptionDescription::OptionValueImplicit, "off" },
  { "-title", "*title",
    gkg::OptionDescription::OptionValueNext, 0 },
  { "-visual", "*visual",
    gkg::OptionDescription::OptionValueNext, 0 },
  { "-visual_id", "*visual_id",
    gkg::OptionDescription::OptionValueNext, 0 },
  { "-xrm", 0,
    gkg::OptionDescription::OptionPropertyNext, 0 },
  { 0, 0, ( gkg::OptionDescription::OptionStyle )0, 0 }

};

gkg::X11SessionImplementation::X11SessionImplementation( gkg::Session* session )
                              : gkg::SessionImplementation( session,
                                                            defaultProperties,
                                                            defaultOptions )
{
}


gkg::X11SessionImplementation::~X11SessionImplementation()
{
}


gkg::X11SessionImplementation* 
gkg::X11SessionImplementation::getImplementation( gkg::Session* session )
{

  return static_cast< X11SessionImplementation* >(
                                                 session->getImplementation() );

}


void 
gkg::X11SessionImplementation::loadGkgDefaults( gkg::Style* style, 
                                                int32_t priority )
{

  style->loadFile( getGkgGUILibDir() + "/app-defaults/gkg", priority );

}


void gkg::X11SessionImplementation::loadApplicationDefaults( gkg::Style* style,
                                                             int32_t priority )
{

  style->loadFile( getXLibDir() + "/X11/app-defaults/" + _className, priority );
  style->loadFile( getGkgGUILibDir() + "/app-defaults/" + _className,
                   priority );
  const char* xResources = getenv( "XAPPLRESDIR" );
  if ( xResources )
  {

    style->loadFile( std::string( xResources ) + "/" + _className, priority );

  }
  else
  {

    style->loadFile( getHome() + "/" + _className, priority );

  }

}


void 
gkg::X11SessionImplementation::loadDisplayDefaults( gkg::Style* style,
                                                    gkg::Display* display,
                                                    int32_t priority )
{

  std::string str;
  if ( display->getDefaults( str ) )
  {

    style->loadList( str, priority );

  }
  else
  {

    style->loadFile( getHome() + "/.Xdefaults", priority );

  }

}


void gkg::X11SessionImplementation::loadEnvironment( gkg::Style* style,
                                                     int32_t priority )
{

  const char* xEnvironment = getenv( "XENVIRONMENT" );
  if ( xEnvironment )
  {

    style->loadFile( xEnvironment, priority );

  }
  else
  {

    style->loadFile( std::string( ".Xdefaults-" ) + gkg::Host::getName(),
                     priority );

  }

}


std::string gkg::X11SessionImplementation::getHome() const
{

  const char* home = getenv( "HOME" );
  if ( !home )
  {

    return ".";

  }
  return std::string( home );

}


std::string gkg::X11SessionImplementation::getGkgGUILibDir() const
{

  std::string gkgGuiLibDir = getHome() + "/lib";
  if ( getenv( "GKG_GUI_LIBDIR" ) )
  {

    gkgGuiLibDir = std::string( getenv( "GKG_GUI_LIBDIR" ) );

  }
  return gkgGuiLibDir;

}


std::string gkg::X11SessionImplementation::getXLibDir() const
{

  std::string xLibDir = "/usr/X11R6/lib";
  if ( getenv( "X_LIBDIR" ) )
  {

    xLibDir = std::string( getenv( "X_LIBDIR" ) );

  }
  return xLibDir;

}

