#include <gkg-communication-plugin/PluginManager.h>
#include <gkg-communication-plugin/Plugin.h>
#include <gkg-core-exception/Exception.h>


gkg::PluginManager::PluginManager()
                   : gkg::Observable()
{
}


gkg::PluginManager::~PluginManager()
{
}


gkg::Plugin* gkg::PluginManager::getPlugin( const std::string& name ) const
{

  try
  {

    // trying to look for plugin among existing ones
    std::map< std::string, gkg::Plugin* >::const_iterator
      p = _plugins.find( name );
    if ( p != _plugins.end() )
    {

      return ( *p ).second;

    }

    throw std::runtime_error( std::string( "unable to find '" ) + name +
                              "' plugin" );
    return 0;

  }
  GKG_CATCH( "gkg::Plugin* gkg::PluginManager::getPlugin( "
             "const std::string& name ) const" );

}


bool gkg::PluginManager::hasPlugin( const std::string& name ) const
{

  try
  {

    // trying to look for plugin among existing ones
    std::map< std::string, gkg::Plugin* >::const_iterator
      p = _plugins.find( name );
    if ( p != _plugins.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "bool gkg::PluginManager::hasPlugin( "
             "const std::string& name ) const" );

}


void gkg::PluginManager::registerPlugin( gkg::Plugin* plugin )
{

  try
  {

    if ( plugin )
    {

      _plugins[ plugin->getName() ] = plugin;
      setChanged();

    }

  }
  GKG_CATCH( "void gkg::PluginManager::registerPlugin( gkg::Plugin* plugin )" );

}


void gkg::PluginManager::unregisterPlugin( gkg::Plugin* plugin )
{

  try
  {

    if ( plugin )
    {

      // trying to look for plugin among existing ones
      std::map< std::string, gkg::Plugin* >::iterator
        p = _plugins.find( plugin->getName() );
      if ( p != _plugins.end() )
      {

        _plugins.erase( p );

      }
      setChanged();

    }

  }
  GKG_CATCH( "void gkg::PluginManager::unregisterPlugin( "
             "gkg::Plugin* plugin )" );

}


