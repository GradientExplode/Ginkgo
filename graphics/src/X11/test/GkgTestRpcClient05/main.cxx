#include <gkg-communication-rpc/RpcWriter.h>
#include <gkg-communication-socket/SocketService.h>
#include <gkg-communication-socket/SocketHeader.h>
#include <gkg-communication-rpc/RpcProlog.h>
#include <gkg-communication-rpc/RpcEpilog.h>
#include <gkg-communication-socket/SocketStream.h>
#include <gkg-communication-rpc/RpcReader.h>
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

  setProlog( "GkgTestRpcServer05" );
  setEpilog( "GkgTestRpcServer05" );

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
  getServer().writePackets( array, count );
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


class Reader : public gkg::RpcReader
{

  public:

    Reader( int32_t port, gkg::SocketService* service );
    ~Reader();

    void closeConnection();

  protected:

    enum
    {

      INTEGER,
      NFCNS

    };

    static void receiveInt( gkg::SocketReader* reader,
                            gkg::SocketHeader& header,
                            gkg::SocketStream& stream );

    void connectionClosed( int32_t fd );
    bool getBinary();

    gkg::SocketService* _service;

};

Reader::Reader( int32_t port, gkg::SocketService* service )
       : gkg::RpcReader( port, getBinary(), false ),
         _service(service)
{

  setProlog( "GkgTestRpcServer05Ack" );
  setEpilog( "GkgTestRpcServer05Ack" );
  addFunction( INTEGER, &Reader::receiveInt );

}


Reader::~Reader()
{
}


void Reader::closeConnection()
{

  _service->quitRunning();

}


void Reader::connectionClosed( int32_t )
{

  _service->quitRunning();

}


void Reader::receiveInt( gkg::SocketReader* reader,
                         gkg::SocketHeader&,
                         gkg::SocketStream& stream )
{

  int32_t i;
  stream >> i;
  std::cerr << "receiveAck: " << i << std::endl;

  Reader* r = dynamic_cast< Reader* >( reader );

  if ( r )
  {

    r->closeConnection();

  }

}


bool Reader::getBinary()
{

  return true;

}


class Service : public gkg::SocketService 
{

  public:

    Service( int32_t port );
    ~Service();

  protected:

    void createReader( int32_t fd );

  protected:

    Reader* _reader;

};


Service::Service( int32_t port )
        : gkg::SocketService( port ),
          _reader( 0 )
{
}


Service::~Service()
{

  delete _reader;

}


void Service::createReader( int32_t fd )
{

  _reader = new Reader( fd, this );

}


//
// main procedure
//
int main( int /*argc*/, char* /*argv*/[] )
{


  timeval tv;
  Writer* writer = new Writer( 8001 );
  Service* service = new Service( 8002 );

  char* array = new char[ ARRAY_SIZE ];

  writer->send( 12 );

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
  writer->flush();

  // wait for an acknowledgment during 10s
  tv.tv_sec = 10;
  tv.tv_usec = 0;
  service->run( &tv );

  delete service;
  delete array;
  delete writer;

  return EXIT_SUCCESS;

}
