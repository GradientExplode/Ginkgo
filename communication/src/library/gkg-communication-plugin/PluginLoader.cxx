#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-communication-plugin/PluginLoaderImplementation.h>
#include <gkg-communication-core/CommunicationImplementationFactory.h>
#include <gkg-core-exception/Exception.h>


gkg::PluginLoader::PluginLoader()
{

  try
  {

    _pluginLoaderImplementation = gkg::getCommunicationImplementationFactory().
                                       createPluginLoaderImplementation( this );

  }
  GKG_CATCH( "gkg::PluginLoader::PluginLoader()" );

}


gkg::PluginLoader::~PluginLoader()
{

  delete _pluginLoaderImplementation;

}


void gkg::PluginLoader::load( bool verbose, bool loadFunctors ) const
{

  try
  {

    _pluginLoaderImplementation->load( verbose, loadFunctors );

  }
  GKG_CATCH( "void gkg::PluginLoader::load( "
             "bool verbose, bool loadFunctors ) const" );

}


void gkg::PluginLoader::loadPlugin( const std::string& pluginName,
                                    bool verbose ) const
{

  try
  {

    _pluginLoaderImplementation->loadPlugin( pluginName, verbose );

  }
  GKG_CATCH( "void gkg::PluginLoader::loadPlugin( "
             "const std::string& pluginName, bool verbose ) const" );

}


void gkg::PluginLoader::addPluginConfigFileName(
                                       const std::string& pluginConfigFileName )
{

  try
  {

  return _pluginLoaderImplementation->addPluginConfigFileName(
                                                         pluginConfigFileName );

  }
  GKG_CATCH( "void gkg::PluginLoader::addPluginConfigFileName( "
             "const std::string& pluginConfigFileName )" );

}

gkg::PluginLoaderImplementation* gkg::PluginLoader::getImplementation() const
{

  return _pluginLoaderImplementation;

}
