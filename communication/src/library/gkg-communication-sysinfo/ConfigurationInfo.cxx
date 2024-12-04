#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/ConfigurationInfoImplementation.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-core/CommunicationImplementationFactory.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-core-io/BaseObjectReader.h>
#include <gkg-core-io/BaseObjectWriter.h>
#include <gkg-core-object/TypedObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>
#include <iostream>
#include <sstream>


gkg::ConfigurationInfo::ConfigurationInfo()
{

  _configurationInfoImplementation = 
                                    gkg::getCommunicationImplementationFactory()
                                 .createConfigurationInfoImplementation( this );

  gkg::Directory directoryGkg( getGkgPath() );
  if ( !directoryGkg.isValid() )
  {

    directoryGkg.mkdir();

  }

  gkg::Directory directoryHiddenGkg( getHiddenGkgPath() );
  if ( !directoryHiddenGkg.isValid() )
  {

    directoryHiddenGkg.mkdir();

  }

}


gkg::ConfigurationInfo::~ConfigurationInfo()
{

  delete _configurationInfoImplementation;

}


////////////////////////////////////////////////////////////////////////////////
// system information(s)
////////////////////////////////////////////////////////////////////////////////

// generally points to $HOME
std::string gkg::ConfigurationInfo::getHomePath() const
{

  return _configurationInfoImplementation->getHomePath();

}


std::string gkg::ConfigurationInfo::getGkgVersion() const
{

  std::ostringstream os;

  os << GKG_VERSION;

  return os.str();

}


std::string gkg::ConfigurationInfo::getGkgVersionAndPatch() const
{

  std::ostringstream os;

  os << GKG_VERSION_AND_PATCH;

  return os.str();

}


// generally points to /usr/share/
std::string gkg::ConfigurationInfo::getSharePath() const
{

  return _configurationInfoImplementation->getSharePath();

}


// generally points to $HOME/.gkg/
std::string gkg::ConfigurationInfo::getHiddenGkgPath() const
{

  return getHomePath() + ".gkg" + gkg::getDirectorySeparator();

}


// generally points to /tmp/
std::string gkg::ConfigurationInfo::getTmpPath() const
{

  return _configurationInfoImplementation->getTmpPath();

}


// returns the $PATH content
std::string gkg::ConfigurationInfo::getEnvPath() const
{

  return _configurationInfoImplementation->getEnvPath();

}


// points by default to /tmp/ but can be overwritten using the 
// GKG_MEMORY_MAPPING_PATH environment variable
std::string gkg::ConfigurationInfo::getMemoryMappingPath() const
{

  return _configurationInfoImplementation->getMemoryMappingPath();

}


////////////////////////////////////////////////////////////////////////////////
// gkg information(s)
////////////////////////////////////////////////////////////////////////////////

// generally points to /usr/share/gkg/
std::string gkg::ConfigurationInfo::getGkgPath() const
{

  if ( getSharePath() == gkg::getDirectorySeparator() )
  {

    return getHiddenGkgPath();

  }
  return getSharePath() + "gkg" + gkg::getDirectorySeparator();

}


// generally points to /usr/share/gkg/icons
std::string gkg::ConfigurationInfo::getIconPath(
                                             const std::string& iconName ) const
{

  return getGkgPath() + "icons" + gkg::getDirectorySeparator() + iconName;

}


// generally points to /usr/share/gkg/ui
std::string gkg::ConfigurationInfo::getUiPath(
                                             const std::string& applicationName,
                                             const std::string& uiName ) const
{

  return getGkgPath() + "ui" + gkg::getDirectorySeparator() +
         applicationName + gkg::getDirectorySeparator() +
         uiName;

}


std::string gkg::ConfigurationInfo::getGkgResourceConfigFileName() const
{

  return getGkgPath() + "gkgrc";

}


std::string gkg::ConfigurationInfo::getGkgPluginConfigFileName() const
{

  return getGkgPath() + "pluginrc";

}


std::string gkg::ConfigurationInfo::getGkgFunctorConfigFileName() const
{

  return getGkgPath() + "functorrc";

}


std::string gkg::ConfigurationInfo::getHeaderExtensionName() const
{

  try
  {

    std::string headerExtensionName = ".minf";

    gkg::File file( gkg::ConfigurationInfo::getInstance().
                                               getGkgResourceConfigFileName() );
    if ( !file.isExisting() )
    {

      createEmptyResourceConfigFile( file );
      file.update();

    }
    if ( file.isUserReadable() )
    {

      gkg::UncommentCounterInputFileStream is( file.getName().c_str() );
      gkg::TypedObject< gkg::Dictionary > resourceDictionary;
      gkg::BaseObjectReader baseObjectReader;
      baseObjectReader.read( is, resourceDictionary );

      if ( !resourceDictionary.hasAttribute( "header_extension_name" ) )
      {

        headerExtensionName = ".minf";

      }
      else
      {
      
        resourceDictionary.getAttribute( "header_extension_name",
                                         headerExtensionName );

      }

    }
    else
    {

      throw std::runtime_error( std::string( "GKG resource config file '" ) +
                                file.getName() + "' is not readable" );

    }

    return headerExtensionName;

  }
  GKG_CATCH( "std::string "
             "gkg::ConfigurationInfo::getHeaderExtensionName() const" );

}


std::string gkg::ConfigurationInfo::getFunctionalVolumeDimension() const
{

  try
  {

    std::string functionalVolumeDimension = "4D";

    gkg::File file( gkg::ConfigurationInfo::getInstance().
                                               getGkgResourceConfigFileName() );
    if ( !file.isExisting() )
    {

      createEmptyResourceConfigFile( file );
      file.update();

    }
    if ( file.isUserReadable() )
    {

      gkg::UncommentCounterInputFileStream is( file.getName().c_str() );
      gkg::TypedObject< gkg::Dictionary > resourceDictionary;
      gkg::BaseObjectReader baseObjectReader;
      baseObjectReader.read( is, resourceDictionary );

      if ( !resourceDictionary.hasAttribute( "functional_volume_dimension" ) )
      {

        functionalVolumeDimension = "4D";

      }
      else
      {
      
        resourceDictionary.getAttribute( "functional_volume_dimension",
                                         functionalVolumeDimension );

      }

    }
    else
    {

      throw std::runtime_error( std::string( "GKG resource config file '" ) +
                                file.getName() + "' is not readable" );

    }

    return functionalVolumeDimension;

  }
  GKG_CATCH( "std::string "
             "gkg::ConfigurationInfo::getFunctionalVolumeDimension() const" );

}


std::string gkg::ConfigurationInfo::getCSVSeparator() const
{

  try
  {

    std::string csvSeparator = " ";

    gkg::File file( gkg::ConfigurationInfo::getInstance().
                                               getGkgResourceConfigFileName() );
    if ( !file.isExisting() )
    {

      createEmptyResourceConfigFile( file );
      file.update();

    }
    if ( file.isUserReadable() )
    {

      gkg::UncommentCounterInputFileStream is( file.getName().c_str() );
      gkg::TypedObject< gkg::Dictionary > resourceDictionary;
      gkg::BaseObjectReader baseObjectReader;
      baseObjectReader.read( is, resourceDictionary );

      if ( !resourceDictionary.hasAttribute( "csv_separator" ) )
      {

        csvSeparator = " ";

      }
      else
      {
      
        resourceDictionary.getAttribute( "csv_separator",
                                         csvSeparator );

      }

    }
    else
    {

      throw std::runtime_error( std::string( "GKG resource config file '" ) +
                                file.getName() + "' is not readable" );

    }

    return csvSeparator;

  }
  GKG_CATCH( "std::string "
             "gkg::ConfigurationInfo::getCSVSeparator() const" );

}


uint64_t gkg::ConfigurationInfo::getMemoryMappingPageSize() const
{

  try
  {

    uint64_t memoryMappingPageSize = 1048576U;   // = 1 MBytes

    gkg::File file( gkg::ConfigurationInfo::getInstance().
                                               getGkgResourceConfigFileName() );
    if ( !file.isExisting() )
    {

      createEmptyResourceConfigFile( file );
      file.update();

    }
    if ( file.isUserReadable() )
    {

      gkg::UncommentCounterInputFileStream is( file.getName().c_str() );
      gkg::TypedObject< gkg::Dictionary > resourceDictionary;
      gkg::BaseObjectReader baseObjectReader;
      baseObjectReader.read( is, resourceDictionary );

      if ( !resourceDictionary.hasAttribute( "memory_mapping_page_size" ) )
      {

        memoryMappingPageSize = 1048576U;

      }
      else
      {

        double memory_mapping_page_size = 0.0;
        resourceDictionary.getAttribute( "memory_mapping_page_size",
                                         memory_mapping_page_size );
        memoryMappingPageSize = ( uint64_t )( memory_mapping_page_size + 0.5 );

      }

    }
    else
    {

      throw std::runtime_error( std::string( "GKG resource config file '" ) +
                                file.getName() + "' is not readable" );

    }

    return memoryMappingPageSize;

  }
  GKG_CATCH( "uint64_t "
             "gkg::ConfigurationInfo::getMemoryMappingPageSize() const" );

}


double gkg::ConfigurationInfo::getMemoryMappingSwitchOnFactor() const
{

  try
  {

    // memory mapping is switched on when 90% of the free RAM is already used
    double memoryMappingSwitchOnFactor = 0.9;

    gkg::File file( gkg::ConfigurationInfo::getInstance().
                                               getGkgResourceConfigFileName() );
    if ( !file.isExisting() )
    {

      createEmptyResourceConfigFile( file );
      file.update();

    }
    if ( file.isUserReadable() )
    {

      gkg::UncommentCounterInputFileStream is( file.getName().c_str() );
      gkg::TypedObject< gkg::Dictionary > resourceDictionary;
      gkg::BaseObjectReader baseObjectReader;
      baseObjectReader.read( is, resourceDictionary );

      if ( !resourceDictionary.hasAttribute(
                                           "memory_mapping_switch_on_factor" ) )
      {

        memoryMappingSwitchOnFactor = 0.9;

      }
      else
      {

        resourceDictionary.getAttribute( "memory_mapping_switch_on_factor",
                                         memoryMappingSwitchOnFactor );

      }

    }
    else
    {

      throw std::runtime_error( std::string( "GKG resource config file '" ) +
                                file.getName() + "' is not readable" );

    }

    return memoryMappingSwitchOnFactor;

  }
  GKG_CATCH( "double "
             "gkg::ConfigurationInfo::getMemoryMappingSwitchOnFactor() const" );

}


bool gkg::ConfigurationInfo::isMemoryMappingVerbose() const
{

  try
  {

    bool memoryMappingVerbosity = false;

    gkg::File file( gkg::ConfigurationInfo::getInstance().
                                               getGkgResourceConfigFileName() );
    if ( !file.isExisting() )
    {

      createEmptyResourceConfigFile( file );
      file.update();

    }
    if ( file.isUserReadable() )
    {

      gkg::UncommentCounterInputFileStream is( file.getName().c_str() );
      gkg::TypedObject< gkg::Dictionary > resourceDictionary;
      gkg::BaseObjectReader baseObjectReader;
      baseObjectReader.read( is, resourceDictionary );

      if ( !resourceDictionary.hasAttribute( "memory_mapping_verbosity" ) )
      {

        memoryMappingVerbosity = false;

      }
      else
      {

        double memory_mapping_verbosity = 0.0;
        resourceDictionary.getAttribute( "memory_mapping_verbosity",
                                         memory_mapping_verbosity );
        memoryMappingVerbosity = ( memory_mapping_verbosity > 0.0 ? true :
                                                                    false );

      }

    }
    else
    {

      throw std::runtime_error( std::string( "GKG resource config file '" ) +
                                file.getName() + "' is not readable" );

    }

    return memoryMappingVerbosity;

  }
  GKG_CATCH( "bool "
             "gkg::ConfigurationInfo::getMemoryMappingVerbosity() const" );

}


bool gkg::ConfigurationInfo::isMemoryMappingDisabled() const
{

  try
  {

    bool memoryMappingDisabled = false;

    gkg::File file( gkg::ConfigurationInfo::getInstance().
                                               getGkgResourceConfigFileName() );
    if ( !file.isExisting() )
    {

      createEmptyResourceConfigFile( file );
      file.update();

    }
    if ( file.isUserReadable() )
    {

      gkg::UncommentCounterInputFileStream is( file.getName().c_str() );
      gkg::TypedObject< gkg::Dictionary > resourceDictionary;
      gkg::BaseObjectReader baseObjectReader;
      baseObjectReader.read( is, resourceDictionary );

      if ( !resourceDictionary.hasAttribute( "memory_mapping_disabled" ) )
      {

        memoryMappingDisabled = false;

      }
      else
      {

        double memory_mapping_disabled = 0.0;
        resourceDictionary.getAttribute( "memory_mapping_disabled",
                                         memory_mapping_disabled );
        memoryMappingDisabled = ( memory_mapping_disabled > 0.0 ? true :
                                                                  false );

      }

    }
    else
    {

      throw std::runtime_error( std::string( "GKG resource config file '" ) +
                                file.getName() + "' is not readable" );

    }

    return memoryMappingDisabled;

  }
  GKG_CATCH( "bool "
             "gkg::ConfigurationInfo::getMemoryMappingVerbosity() const" );

}


int32_t gkg::ConfigurationInfo::getNumberOfCpuToUse() const
{

  try
  {

    int32_t numberOfCpuToUse = 0;

    gkg::File file( gkg::ConfigurationInfo::getInstance().
                                               getGkgResourceConfigFileName() );
    if ( !file.isExisting() )
    {

      createEmptyResourceConfigFile( file );
      file.update();

    }
    if ( file.isUserReadable() )
    {

      gkg::UncommentCounterInputFileStream is( file.getName().c_str() );
      gkg::TypedObject< gkg::Dictionary > resourceDictionary;
      gkg::BaseObjectReader baseObjectReader;
      baseObjectReader.read( is, resourceDictionary );

      if ( !resourceDictionary.hasAttribute( "number_of_cpu_to_use" ) )
      {

        numberOfCpuToUse = 0;

      }
      else
      {

        double number_of_cpu_to_use = 0.0;
        resourceDictionary.getAttribute( "number_of_cpu_to_use", 
                                         number_of_cpu_to_use );
        numberOfCpuToUse = (int32_t)( number_of_cpu_to_use + 0.5 );

      }

    }
    else
    {

      throw std::runtime_error( std::string( "GKG resource config file '" ) +
                                file.getName() + "' is not readable" );

    }

    return numberOfCpuToUse;

  }
  GKG_CATCH( "int32_t gkg::ConfigurationInfo::getNumberOfCpuToUse() const" );

}


void gkg::ConfigurationInfo::setAnonymousName( bool value )
{

  _configurationInfoImplementation->setAnonymousName( value );

}


bool gkg::ConfigurationInfo::hasAnonymousName() const
{

  return _configurationInfoImplementation->hasAnonymousName();

}


////////////////////////////////////////////////////////////////////////////////
// gkg-private information(s)
////////////////////////////////////////////////////////////////////////////////

// generally points to /usr/share/gkg-private/
std::string gkg::ConfigurationInfo::getGkgPrivatePath() const
{

  if ( getSharePath() == gkg::getDirectorySeparator() )
  {

    return getHiddenGkgPath();

  }
  return getSharePath() + "gkg-private" + gkg::getDirectorySeparator();

}


std::string gkg::ConfigurationInfo::getGkgPrivatePluginConfigFileName() const
{

  return getGkgPrivatePath() + "pluginrc";

}


std::string gkg::ConfigurationInfo::getGkgPrivateFunctorConfigFileName() const
{

  return getGkgPrivatePath() + "functorrc";

}


////////////////////////////////////////////////////////////////////////////////
// gkg-data information(s)
////////////////////////////////////////////////////////////////////////////////

// generally points to /usr/share/gkg-data/
std::string gkg::ConfigurationInfo::getGkgDataPath() const
{

  if ( getSharePath() == gkg::getDirectorySeparator() )
  {

    return getHiddenGkgPath();

  }
  return getSharePath() + "gkg-data" + gkg::getDirectorySeparator();

}


std::string 
gkg::ConfigurationInfo::getAnatomicalTemplatesPath(
                                         const std::string& specy,
                                         const std::string& templateName ) const
{

  return getGkgDataPath() + 
         "anatomical-templates" + gkg::getDirectorySeparator() +
         specy + gkg::getDirectorySeparator() +
         templateName +
         gkg::getDirectorySeparator();

}


std::string 
gkg::ConfigurationInfo::getWhiteMatterAtlasesPath(
                                            const std::string& specy,
                                            const std::string& atlasName ) const
{

  return getGkgDataPath() + 
         "white-matter-atlases" + gkg::getDirectorySeparator() +
         specy + gkg::getDirectorySeparator() +
         atlasName +
         gkg::getDirectorySeparator();

}


std::string 
gkg::ConfigurationInfo::getMedusaResourcePath() const
{

  return getGkgDataPath() + 
         "medusa";

}


std::set< std::string > 
gkg::ConfigurationInfo::getMedusaNeuronModelPaths(
                                     const std::string& neuronDatabaseName,
                                     const std::string& neuronTypeString ) const
{

  try
  {

    gkg::Directory directory( getMedusaResourcePath() +
                              gkg::getDirectorySeparator() +
                              "neuron-models" +
                              gkg::getDirectorySeparator() +
                              neuronDatabaseName +
                              gkg::getDirectorySeparator() +
                              neuronTypeString );
    std::set< std::string > medusaNeuronModelPaths;


    if ( directory.isValid() )
    {

      std::set< std::string > 
       fileNames = directory.getMatchingFiles( "*.*.medusageometry" );

      if ( fileNames.empty() )
      {

       fileNames = directory.getMatchingFiles( "*.medusageometry" );

      }

      std::set< std::string >::const_iterator
        f = fileNames.begin(),
        fe = fileNames.end();
      while ( f != fe )
      {

        medusaNeuronModelPaths.insert( directory.getPath() +
                                       gkg::getDirectorySeparator() +
                                       *f );                                     
        ++ f;

      }

    }
    else
    {

      throw std::runtime_error(
                     std::string( "unable to find neuron database '" ) +
                     neuronDatabaseName + "' of type '" +
                     neuronTypeString );

    }

    return medusaNeuronModelPaths;

  }
  GKG_CATCH( "std::set< std::string > "
             "gkg::ConfigurationInfo::getMedusaNeuronModelPaths() const" );

}

////////////////////////////////////////////////////////////////////////////////
// further private method(s)
////////////////////////////////////////////////////////////////////////////////

void gkg::ConfigurationInfo::createEmptyResourceConfigFile(
                                                   const gkg::File& file ) const
{

  std::ofstream os( file.getName().c_str() );

  os << "attributes = {" << std::endl
     << "  'header_extension_name' : '.minf'" << std::endl
     << "}" << std::endl;

  os.close();

}


////////////////////////////////////////////////////////////////////////////////
// access to implementation class
////////////////////////////////////////////////////////////////////////////////

gkg::ConfigurationInfoImplementation*
gkg::ConfigurationInfo::getImplementation() const
{

  return _configurationInfoImplementation;

}
