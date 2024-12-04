// Directory class

#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-log/HistoryWriter_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    char buffer[ 12 ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

    std::cout << "hidden gkg path : "
              << gkg::ConfigurationInfo::getInstance().getHiddenGkgPath()
              << std::endl;

    gkg::HistoryWriter::getInstance().initialize( argv[ 0 ] );

    gkg::HistoryWriter::getInstance().add( "Initialized" );
    gkg::HistoryWriter::getInstance().add( "Test text addition" );
    gkg::HistoryWriter::getInstance().add( "Command1", 23 );
    gkg::HistoryWriter::getInstance().add( "Command2", 45.23, false );
    gkg::HistoryWriter::getInstance().add( "", -13.8, false );
    gkg::HistoryWriter::getInstance().add( "", 0.3 );
    gkg::HistoryWriter::getInstance().add( "", 1.0, false );
    gkg::HistoryWriter::getInstance().add( "", 2 );
    gkg::HistoryWriter::getInstance().add( "Image 12", 
                                           "Image12.data", 
                                           buffer,
                                           12 );
    gkg::HistoryWriter::getInstance().add( "Done" );

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
