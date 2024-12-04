#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-communication-rpc/RpcWriter.h>
#include <gkg-communication-socket/SocketService.h>
#include <gkg-communication-socket/SocketHeader.h>
#include <gkg-communication-rpc/RpcProlog.h>
#include <gkg-communication-rpc/RpcEpilog.h>
#include <gkg-communication-socket/SocketStream.h>
#include <gkg-core-exception/Exception.h>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>


class Writer : public gkg::RpcWriter
{

  public:

    enum
    {

      STRING,
      INTEGER,
      CHAR_ARRAY,
      IMAGE,
      NFCNS

    };

    Writer( const std::string& host, in_port_t port );
    void send( char* msg );
    void send( int32_t i );
    void send( char* array, int32_t count );
    void send( char* array, int32_t dimX, int32_t dimY, int32_t bpp );
    void flush();

  protected:

    bool getFatal();
    bool getBinary();

};


Writer::Writer( const std::string& host, in_port_t port )
       : gkg::RpcWriter( host, port, getFatal(), getBinary() )
{

  setProlog( "EvaRpcWriter" );
  setEpilog( "EvaRpcWriter" );

}


void Writer::flush()
{

  getServer().flush();

}


void Writer::send( char* msg )
{

  gkg::SocketHeader header( this, Writer::STRING );
  gkg::RpcProlog prolog( _prolog );
  gkg::RpcEpilog epilog( _epilog );
  getServer() << prolog << header << msg << epilog;

}


void Writer::send( int32_t i )
{

  gkg::SocketHeader header( this, Writer::INTEGER );
  gkg::RpcProlog prolog( _prolog );
  gkg::RpcEpilog epilog( _epilog );
  getServer() << prolog << header << i << epilog;

}


void Writer::send( char* array, int32_t count )
{

  gkg::SocketHeader header( this, Writer::CHAR_ARRAY );
  gkg::RpcProlog prolog( _prolog );
  gkg::RpcEpilog epilog( _epilog );
  getServer() << prolog << header << count;
  getServer().writePackets( array, count );
  getServer() << epilog;

}


void Writer::send( char* array, int32_t dimX, int32_t dimY, int32_t bpp )
{

  gkg::SocketHeader header( this, Writer::IMAGE );
  gkg::RpcProlog prolog( _prolog );
  gkg::RpcEpilog epilog( _epilog );
  getServer() << prolog << header << dimX << dimY << bpp;
  getServer().writePackets( array, dimX * dimY * bpp );
  getServer() << epilog;

}


bool Writer::getFatal()
{

  return false;

}


bool Writer::getBinary()
{

  return true;

}


//
// main procedure
//

int main( int32_t argc, char* argv[] )
{

  int32_t result = EXIT_SUCCESS;

  try
  {

    std::string fileName;
    std::string host( "localhost" );
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( argc, argv, "Test for Eva Functor viewer" );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileName );
    application.addSingleOption( "-host",
                                 "Server IP address"
				 "(default=localhost)",
                                 host,
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

    Writer* writer = new Writer( host, 8001 );
    
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
   
    writer->send( ( char* )"reset" );
    writer->flush();

    ////////////////////////////////////////////////////////////////////////////
    // reading data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileName << "' : " << std::flush;

    }
    gkg::Volume< int16_t > t2;
    gkg::Reader::getInstance().read( fileName, t2 );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // sending data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "Sending images : " << std::flush;

    }

    int32_t z;
    int32_t sizeX = t2.getSizeX();
    int32_t sizeY = t2.getSizeY();
    int32_t sizeZ = t2.getSizeZ();

    writer->send( ( char* )"reset" );

    for ( z = 0; z < sizeZ; z++ )
    {

      if ( z )
      {

        std::cout << gkg::Eraser( 22 );

      }

      if ( verbose )
      {

        std::cout << " [ " << std::setw( 7 ) << z + 1 
                  << " / " << std::setw( 7 ) << sizeZ
                  << " ]" << std::flush;

      }

      writer->send( ( char* )&t2( 0, 0, z ), 
                    sizeX, sizeY, (int32_t)sizeof( int16_t ) );
      writer->flush();

    }

    if ( verbose )
    {

      std::cout << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // deleting RPC writer
    ////////////////////////////////////////////////////////////////////////////

    delete writer;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
