#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    bool verbose = false;

    gkg::Application application( argc, argv,
                                  "Test for partial GIS reading" );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    gkg::TypedDiskFormat< gkg::Volume< int16_t > >*
      format = gkg::DiskFormatFactory< gkg::Volume< int16_t > >::getInstance().
                getDiskFormat( "gis" );

    gkg::Volume< int16_t > volumeOut;

    gkg::BoundingBox< int32_t > boundingBox( 0, 64, 0, 64, 64, 64 );

    std::cout << "x : "; 
    std::cout << boundingBox.getLowerX() << " -> ";
    std::cout << boundingBox.getUpperX() << std::endl;
    std::cout << "y : "; 
    std::cout << boundingBox.getLowerY() << " -> ";
    std::cout << boundingBox.getUpperY() << std::endl;
    std::cout << "z : "; 
    std::cout << boundingBox.getLowerZ() << " -> ";
    std::cout << boundingBox.getUpperZ() << std::endl;
    std::cout << "t : "; 
    std::cout << boundingBox.getLowerT() << " -> ";
    std::cout << boundingBox.getUpperT() << std::endl;

    gkg::VolumeProxy< int16_t > volumeProxy;
    format->preparePartialBinaryRead( fileNameIn, volumeProxy );
    format->partialBinaryRead( volumeProxy, volumeOut, boundingBox );
    format->write( fileNameOut, volumeOut, false );

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

