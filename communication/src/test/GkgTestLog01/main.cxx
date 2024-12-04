// Directory class

#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-log/LogWriter.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::cout << "hidden gkg path : "
              << gkg::ConfigurationInfo::getInstance().getHiddenGkgPath()
              << std::endl;

    gkg::LogWriter::getInstance().initialize( argv[ 0 ] );

    gkg::LogWriter::getInstance().add( "Initialized" );
    gkg::LogWriter::getInstance().add( "One dummy log" );
    gkg::LogWriter::getInstance().add( "Done" );

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
