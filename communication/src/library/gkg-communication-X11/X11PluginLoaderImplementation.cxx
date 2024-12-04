#include <gkg-communication-X11/X11PluginLoaderImplementation.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-core-cppext/StringStream.h>
#include <gkg-core-exception/Exception.h>
#include <dlfcn.h>
#include <iostream>


gkg::X11PluginLoaderImplementation::X11PluginLoaderImplementation(
                                               gkg::PluginLoader* pluginLoader )
                                  : gkg::PluginLoaderImplementation( 
                                                                 pluginLoader )
{
}


gkg::X11PluginLoaderImplementation::~X11PluginLoaderImplementation()
{

  if ( !_handles.empty() )
  {

    std::list< void* >::iterator
      h = _handles.begin(),
      he = _handles.end();

    while ( h != he )
    {

      dlclose( *h );
      ++h;

    }

  }

}


void gkg::X11PluginLoaderImplementation::loadPlugin(
                                                  const std::string& pluginName,
                                                  bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "loading plugin '" << pluginName << "': " << std::flush;

    }
    void* handle = dlopen( pluginName.c_str(), RTLD_GLOBAL | RTLD_NOW );
    if ( !handle )
    {

      if ( verbose )
      {

        std::ostringstream os;
        os << dlerror();

        std::cout << "failed" << std::endl;
        std::cout << "........................................"
                  << "........................................"
                  << std::endl;
        std::cout << "    " << os.str() << std::endl;
        std::cout << "........................................"
                  << "........................................"
                  << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      _handles.push_back( handle );

    }

  }
  GKG_CATCH( "void gkg::X11PluginLoaderImplementation::loadPlugin( "
             "const std::string& pluginName ) const" );

}


gkg::X11PluginLoaderImplementation*
gkg::X11PluginLoaderImplementation::getImplementation(
                                               gkg::PluginLoader* pluginLoader )
{

  return static_cast< gkg::X11PluginLoaderImplementation* >(
                                            pluginLoader->getImplementation() );

}


