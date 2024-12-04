#include <gkg-communication-X11/X11ConfigurationInfoImplementation.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <cstdlib>


//
// return system-dependant paths list separator
//

std::string gkg::getEnvironmentPathSeparator()
{

  return ":";

}


gkg::X11ConfigurationInfoImplementation::X11ConfigurationInfoImplementation(
                                     gkg::ConfigurationInfo* configurationInfo )
                     : gkg::ConfigurationInfoImplementation( configurationInfo )
{
}


gkg::X11ConfigurationInfoImplementation::~X11ConfigurationInfoImplementation()
{
}


std::string gkg::X11ConfigurationInfoImplementation::getHomePath() const
{

  std::string homePathString = "/";
  const char* homePath = getenv( "HOME" );
  if ( homePath )
  {

    homePathString = std::string( homePath );

  }
  else
  {

    homePath = getenv( "HOMEPATH" );
    if ( homePath )
    {

      homePathString = std::string( homePath );

    }

  }
  if ( homePathString[ homePathString.length() - 1U ] != '/' )
  {

    homePathString += '/';

  }
  return homePathString;

}


std::string gkg::X11ConfigurationInfoImplementation::getSharePath() const
{

  std::string sharePathString = "/usr/share";
  const char* sharePath = getenv( "GKG_SHARE_PATH" );
  if ( sharePath )
  {

    sharePathString = std::string( sharePath );

  }
  if ( sharePathString[ sharePathString.length() - 1U ] != '/' )
  {

    sharePathString += '/';

  }
  return sharePathString;

}


std::string gkg::X11ConfigurationInfoImplementation::getTmpPath() const
{

  std::string tmpPathString = "/tmp";
  const char* tmpPath = getenv( "GKG_TMP_PATH" );
  if ( tmpPath )
  {

    tmpPathString = std::string( tmpPath );

  }
  else
  {

    tmpPath = getenv( "TEMP" );
    if ( tmpPath )
    {

      tmpPathString = std::string( tmpPath );

    }
    else
    {

      tmpPath = getenv( "TMP" );
      if ( tmpPath )
      {

        tmpPathString = std::string( tmpPath );

      }

    }

  }
  if ( tmpPathString[ tmpPathString.length() - 1U ] != '/' )
  {

    tmpPathString += '/';

  }
  return tmpPathString;

}


std::string gkg::X11ConfigurationInfoImplementation::getEnvPath() const
{

  std::string envPathString = "";

  const char* envPath = getenv( "PATH" );
  if ( envPath )
  {

    envPathString = std::string( envPath );

  }

  return envPathString;

}


std::string 
gkg::X11ConfigurationInfoImplementation::getMemoryMappingPath() const
{

  std::string memoryMappingPathString = getTmpPath();
  const char* memoryMappingPath = getenv( "GKG_MEMORY_MAPPING_PATH" );
  if ( memoryMappingPath )
  {

    memoryMappingPathString = std::string( memoryMappingPath );

  }
  if ( memoryMappingPathString[ memoryMappingPathString.length() - 1U ] != '/' )
  {

    memoryMappingPathString += '/';

  }
  return memoryMappingPathString;

}


gkg::X11ConfigurationInfoImplementation*
gkg::X11ConfigurationInfoImplementation::getImplementation(
                                     gkg::ConfigurationInfo* configurationInfo )
{

  return static_cast< gkg::X11ConfigurationInfoImplementation* >(
                                       configurationInfo->getImplementation() );

}
