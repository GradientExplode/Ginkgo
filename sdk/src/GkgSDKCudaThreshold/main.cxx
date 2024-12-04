#include <CudaThresholding.h>

#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <string>
#include <iostream>


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    float threshold;
    bool verbose = false;

    std::cout << "Build from SDK" << std::endl;

    gkg::Application application( argc, argv,
                                  "Cuda thesholding "
                                  "(in greater or equal mode)" );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-t",
                                 "Threshold",
                                 threshold );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();         


    if ( verbose )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< float > volumeIn;
    gkg::TypedVolumeReaderProcess< float > volumeReaderProcess( volumeIn );
    volumeReaderProcess.execute( fileNameIn );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "thesholding : " << std::flush;

    }
    CudaThreshold( volumeIn, threshold );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "writing '" << fileNameOut << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut, volumeIn );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
