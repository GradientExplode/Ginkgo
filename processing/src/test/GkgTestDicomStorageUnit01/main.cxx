#include <gkg-processing-plugin-functors/DicomStorageUnit/DicomStorageUnitCommand.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-exception/Exception.h>


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    gkg::RCPointer< gkg::DicomStorageUnitCommand > command(
                  new gkg::DicomStorageUnitCommand( argc, argv, true, false ) );

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
