#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-communication-socket/SocketFrameWriter.h>
#include <gkg-communication-socket/SocketTransferBufferFrame_i.h>
#include <gkg-processing-socket/SocketTransferVector3dFrame_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>


//
// main procedure
//
int main( int argc, char* argv[] )
{

  int32_t result = EXIT_SUCCESS;

  try
  {

    std::string fileName;
    std::string host( "localhost" );
    int32_t writerPort = 8001;
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

    gkg::SocketFrameWriter writer( host, writerPort );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

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

    gkg::Vector3d< int32_t > sizes( volume.getSizeX(), 
                                    volume.getSizeY(), 
                                    volume.getSizeZ() );
    gkg::Vector3d< double > res( 1.0, 1.0, 1.0 );
    volume.getResolution( res );

    gkg::SocketTransferVector3dFrame< int32_t > dimensions( 0, sizes );
    gkg::SocketTransferVector3dFrame< double > resolutions( 1, res );
    gkg::SocketTransferBufferFrame< float > data( 2, 
                                                  &volume( 0 ),
                                                  volume.getSizeXYZ() );

    writer.send( dimensions );
    writer.send( resolutions );
    writer.send( data );
    writer.flush();

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
