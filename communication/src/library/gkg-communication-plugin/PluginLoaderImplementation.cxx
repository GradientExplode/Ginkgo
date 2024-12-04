#include <gkg-communication-plugin/PluginLoaderImplementation.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-core-io/BaseObjectReader.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


gkg::PluginLoaderImplementation::PluginLoaderImplementation(
                                               gkg::PluginLoader* pluginLoader )
                                : _target( pluginLoader )
{

  try
  {

    // pluginrc file
    gkg::File pluginFile( gkg::ConfigurationInfo::getInstance().
                                                 getGkgPluginConfigFileName() );
    if ( !pluginFile.isExisting() )
    {

      createEmptyPluginConfigFile( pluginFile );
      pluginFile.update();

    }
    if ( pluginFile.isUserReadable() )
    {

      _pluginConfigFileNames.push_back( pluginFile.getName() );

    }
    else
    {

      throw std::runtime_error( std::string( "plugin config file '" ) +
                                pluginFile.getName() + "' is not readable" );

    }

    // private pluginrc file
    gkg::File privatePluginFile( gkg::ConfigurationInfo::getInstance().
                                          getGkgPrivatePluginConfigFileName() );
    if ( privatePluginFile.isExisting() && privatePluginFile.isUserReadable() )
    {

      _pluginConfigFileNames.push_back( privatePluginFile.getName() );

    }

    // functorrc file
    gkg::File functorFile( gkg::ConfigurationInfo::getInstance().
                                                getGkgFunctorConfigFileName() );

    if ( functorFile.isExisting() && functorFile.isUserReadable() )
    {

      _functorConfigFileNames.push_back( functorFile.getName() );

    }

    // private functorrc file
    gkg::File privateFunctorFile( gkg::ConfigurationInfo::getInstance().
                                         getGkgPrivateFunctorConfigFileName() );

    if ( privateFunctorFile.isExisting() && 
         privateFunctorFile.isUserReadable() )
    {

      _functorConfigFileNames.push_back( privateFunctorFile.getName() );

    }

  }
  GKG_CATCH( "gkg::PluginLoaderImplementation::PluginLoaderImplementation( "
             "gkg::PluginLoader* pluginLoader )" );

}


gkg::PluginLoaderImplementation::~PluginLoaderImplementation()
{
}


gkg::PluginLoader* gkg::PluginLoaderImplementation::getTarget() const
{

  return _target;

}


void gkg::PluginLoaderImplementation::load( bool verbose,
                                            bool loadFunctors ) const
{

  try
  {

    std::list< std::string >::const_iterator n = _pluginConfigFileNames.begin(),
                                             ne = _pluginConfigFileNames.end();
    while ( n != ne )
    {

      loadPluginConfigFile( *n, verbose );
      ++ n;

    }

    if ( loadFunctors )
    {

      n = _functorConfigFileNames.begin();
      ne = _functorConfigFileNames.end();

      while ( n != ne )
      {

        loadPluginConfigFile( *n, verbose );
        ++ n;

      }

    }

  }
  GKG_CATCH( "void gkg::PluginLoaderImplementation::load() const" );

}


void gkg::PluginLoaderImplementation::addPluginConfigFileName(
                                 const std::string& pluginConfigFileName )
{

  try
  {

    std::list< std::string >::const_iterator n = _pluginConfigFileNames.begin(),
                                             ne = _pluginConfigFileNames.end();
    while ( n != ne )
    {

      if ( pluginConfigFileName == *n )
      {

        throw std::runtime_error( std::string( "plugin configuration file '" ) +
                                  pluginConfigFileName + "' is already added" );

      }
      ++ n;

    }
    _pluginConfigFileNames.push_back( pluginConfigFileName );

  }
  GKG_CATCH( "void gkg::PluginLoaderImplementation::addPluginConfigFileName( "
             "const std::string& pluginConfigFileName ) const" );

}


void gkg::PluginLoaderImplementation::createEmptyPluginConfigFile(
                                                         const gkg::File& file )
{

  std::ofstream os( file.getName().c_str() );

  os << "attributes = {" << std::endl
     << "  'plugins' : [" << std::endl
     << "  ]" << std::endl
     << "}" << std::endl;

  os.close();

}


void gkg::PluginLoaderImplementation::loadPluginConfigFile(
                                        const std::string& pluginConfigFileName,
                                        bool verbose ) const
{

  try
  {

    gkg::File file( pluginConfigFileName );
    if ( file.isUserReadable() )
    {

      gkg::UncommentCounterInputFileStream is( file.getName().c_str() );
      gkg::TypedObject< gkg::Dictionary > pluginDictionary;
      gkg::BaseObjectReader baseObjectReader;
      baseObjectReader.read( is, pluginDictionary );

      std::string attributeName = "plugins";

      gkg::GenericObjectList genericObjectList;
      pluginDictionary.getAttribute( attributeName, genericObjectList );

      gkg::GenericObjectList::const_iterator p = genericObjectList.begin(),
                                             pe = genericObjectList.end();
      std::string pluginName;
      std::string versionExtension = std::string( "." ) +
         gkg::ConfigurationInfo::getInstance().getGkgVersionAndPatch();
      while ( p != pe )
      {

        pluginName = ( *p )->getValue< std::string >() + versionExtension;
        loadPlugin( pluginName, verbose );
        ++ p;

      }

    }

  }
  GKG_CATCH( "void gkg::PluginLoaderImplementation::loadPluginConfigFile( "
             "const std::string& pluginConfigFileName, "
             "bool verbose ) const" );

}
