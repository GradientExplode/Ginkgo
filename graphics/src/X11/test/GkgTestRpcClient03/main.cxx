#include <gkg-communication-rpc/RpcWriter.h>
#include <gkg-communication-socket/SocketService.h>
#include <gkg-communication-socket/SocketHeader.h>
#include <gkg-communication-rpc/RpcProlog.h>
#include <gkg-communication-rpc/RpcEpilog.h>
#include <gkg-communication-socket/SocketStream.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

#define ARRAY_SIZE 16384000LU


class Writer : public gkg::RpcWriter
{

  public:

    enum
    {

      STRING,
      INTEGER,
      CHAR_ARRAY,
      NFCNS

    };

    Writer( in_port_t port );
    void send( char* msg );
    void send( int32_t i );
    void send( char* array, int32_t count );
    void flush();

  protected:

    bool getFatal();
    bool getBinary();

};


Writer::Writer( in_port_t port )
       : gkg::RpcWriter( "localhost", port, getFatal(), getBinary() )
{

  setProlog( "GkgTestRpcServer03" );
  setEpilog( "GkgTestRpcServer03" );

}


void Writer::flush()
{

  getServer().flush();

}


void Writer::send( char* msg )
{

  gkg::SocketHeader header( this, STRING );
  gkg::RpcProlog prolog( _prolog );
  gkg::RpcEpilog epilog( _epilog );
  getServer() << prolog << header << msg << epilog;

}


void Writer::send( int32_t i )
{

  gkg::SocketHeader header( this, INTEGER );
  gkg::RpcProlog prolog( _prolog );
  gkg::RpcEpilog epilog( _epilog );
  getServer() << prolog << header << i << epilog;

}


void Writer::send( char* array, int32_t count )
{

  gkg::SocketHeader header( this, CHAR_ARRAY );
  gkg::RpcProlog prolog( _prolog );
  gkg::RpcEpilog epilog( _epilog );
  getServer() << prolog << header << count;
  getServer().write( array, count );
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
int main( int /*argc*/, char* /*argv*/[] )
{


  Writer* writer = new Writer( 8001 );

  char* array = new char[ ARRAY_SIZE ];

  for ( int32_t k = 0; k < 20; k++ )
  {


    std::cout << k << std::endl;
    uint32_t i;
    for ( i = 0; i < ARRAY_SIZE; i++ )
    {

      array[ i ] = ( char )( ( k + i ) % 128 );

    }
    writer->send( array, ARRAY_SIZE );

  }
  writer->send( ( char* )"exit" );

  delete array;
  delete writer;

  return EXIT_SUCCESS;

}
