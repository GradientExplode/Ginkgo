#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-communication-socket/SocketWriter.h>
#include <gkg-communication-socket/SocketFrameWriter.h>
#include <gkg-communication-socket/SocketTransferBufferFrame_i.h>
#include <gkg-processing-socket/SocketTransferVector3dFrame_i.h>
#include <gkg-core-io/Reader_i.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <stdint.h>


enum StimCodeCommand
{

  CODE_DIMENSION  = 0,
  CODE_RESOLUTION = 1,
  CODE_VOLUME     = 2

};


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string fileName;
    std::string host( "localhost" );
    int32_t writerPort = 8001;
    float delay = -1;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( argc, argv, "Test for socket transfer" );
    application.addSingleOption( "-i",
                                 "Input file name (must be FLOAT)",
                                 fileName );
    application.addSingleOption( "-host",
                                 "Server IP address"
                                 "(default=localhost)",
                                 host,
                                 true );
    application.addSingleOption( "-writerPort",
                                 "Server Port number to write on "
                                 "(default=8001)",
                                 writerPort,
                                 true );
    application.addSingleOption( "-delay",
                                 "Delay in s between 2 volumes (FLOAT)"
                                 "(default=TR)",
                                 delay,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();

    ////////////////////////////////////////////////////////////////////////////
    // initializing RPC stream and commands
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "starting RPC writer : " << std::flush;

    }

    if ( verbose )
    {

     std::cout << "reading '" << fileName << "' : " << std::flush;

    }

    gkg::Volume< float > volume;
            gkg::Reader::getInstance().read( fileName, volume );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    int32_t tabDimensions[ 4 ];
    tabDimensions[ 0 ] = volume.getSizeX();
    tabDimensions[ 1 ] = volume.getSizeY();
    tabDimensions[ 2 ] = volume.getSizeZ();
    tabDimensions[ 3 ] = volume.getSizeT();

    double resolutionT;
    gkg::Vector3d< double > res( 1.0, 1.0, 1.0 );
    volume.getResolution( res, resolutionT );

    double tabResolutions[ 4 ];
    tabResolutions[ 0 ] = res.x;
    tabResolutions[ 1 ] = res.y;
    tabResolutions[ 2 ] = res.z;
    tabResolutions[ 3 ] = resolutionT;


    // Set delay after sending each volume
    if( delay == -1 )
    {

      delay = resolutionT;

    }

    for ( int32_t t = 0; t < volume.getSizeT(); ++t )
    {

      if ( t == 0 )
      {

        {

          gkg::SocketTransferBufferFrame< int32_t > dimensions( CODE_DIMENSION,
                                                                tabDimensions,
                                                                4 );
          gkg::SocketFrameWriter writer( host, writerPort );
          std::cout << "send dimensions" << std::endl;
          writer.send( dimensions );

        }

        {

          gkg::SocketTransferBufferFrame< double > resolutions( CODE_RESOLUTION,
                                                                tabResolutions,
                                                                4 );
          gkg::SocketFrameWriter writer( host, writerPort );
          std::cout << "send resolutions" << std::endl;
          writer.send( resolutions );

        }

      }

      gkg::SocketTransferBufferFrame< float > data( CODE_VOLUME,
                                                    &volume( 0, 0, 0, t ),
                                                    volume.getSizeXYZ() );
      gkg::SocketFrameWriter writer( host, writerPort );
      std::cout << "send data ( " << t + 1 << " / "
                << volume.getSizeT() << " )" << std::endl;
      writer.send( data );

      usleep( (int32_t)( delay * 1000000.0 ) );

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
