#include <gkg-communication-rpc/RpcReader.h>
#include <gkg-communication-socket/SocketService.h>
#include <gkg-communication-socket/SocketHeader.h>
#include <gkg-communication-socket/SocketStream.h>
#include <gkg-communication-rpc/RpcWriter.h>
#include <gkg-communication-rpc/RpcProlog.h>
#include <gkg-communication-rpc/RpcEpilog.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>




//
// Writer
//


class Writer : public gkg::RpcWriter
{

  public:

    enum
    {

      INTEGER,
      NFCNS

    };

    Writer( in_port_t port );
    void send( int32_t i );
    void flush();

  protected:

    bool getFatal();
    bool getBinary();

};


Writer::Writer( in_port_t port )
       : gkg::RpcWriter( "localhost", port, getFatal(), getBinary() )
{

  setProlog( "GkgTestRpcServer05Ack" );
  setEpilog( "GkgTestRpcServer05Ack" );

}


void Writer::flush()
{

  getServer().flush();

}


void Writer::send( int32_t i )
{

  gkg::SocketHeader header( this, INTEGER );
  gkg::RpcProlog prolog( _prolog );
  gkg::RpcEpilog epilog( _epilog );
  getServer() << prolog << header << i << epilog;

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

    Reader( in_port_t port, gkg::SocketService* service );
    ~Reader();

  protected:

    enum
    {

      STRING,
      INTEGER,
      CHAR_ARRAY,
      NFCNS

    };

    static void receiveString( gkg::SocketReader* reader,
                               gkg::SocketHeader& header,
                               gkg::SocketStream& stream );
    static void receiveInt( gkg::SocketReader* reader,
                            gkg::SocketHeader& header,
                            gkg::SocketStream& stream );
    static void receiveCharArray( gkg::SocketReader* reader,
                                  gkg::SocketHeader& header,
                                  gkg::SocketStream& stream );

    void connectionClosed( int32_t fd );
    bool getBinary();

    gkg::SocketService* _service;

};


class Service : public gkg::SocketService 
{

  public:

    Service( in_port_t port );
    ~Service();

  protected:

    void createReader( int32_t fd );

  protected:

    Reader* _reader;

};


//
// class Reader
//

// the reader is set to be blocking, because huge amount of memory may
// be transfered from client to server and none should be missed!

Reader::Reader( in_port_t port, gkg::SocketService* service )
       : gkg::RpcReader( port, getBinary(), false ),
         _service(service)
{

  setProlog( "GkgTestRpcServer05" );
  setEpilog( "GkgTestRpcServer05" );
  addFunction( STRING, &Reader::receiveString );
  addFunction( INTEGER, &Reader::receiveInt );
  addFunction( CHAR_ARRAY, &Reader::receiveCharArray );

}


Reader::~Reader()
{
}


void Reader::connectionClosed( int32_t )
{

  _service->quitRunning();

}


void Callback( char* text );  // The user must supply this routine.


void Reader::receiveString( gkg::SocketReader*,
                            gkg::SocketHeader&,
                            gkg::SocketStream& stream )
{

  char text[ 80 ];
  stream >> text;
  Callback( text );

  if ( strcmp( text, "exit" ) == 0 )
  {

    Writer* writer = new Writer( 8002 );
    writer->send( 33 );
    delete writer;

  }

}


void Reader::receiveInt( gkg::SocketReader*,
                         gkg::SocketHeader&,
                         gkg::SocketStream& stream )
{

  int32_t i;
  stream >> i;
  std::cerr << "receiveInt: " << i << std::endl;

}


void Reader::receiveCharArray( gkg::SocketReader*,
                               gkg::SocketHeader&,
                               gkg::SocketStream& stream )
{

  int32_t count = 0;
  stream >> count;

  std::cout << "count=" << count << " : ";

  char* array = new char[ count ];

  stream.readPackets( array, count );

  std::cout << "{"
            << std::setw( 3 ) << ( int32_t )array[ 0 ] << " "
            << std::setw( 3 ) << ( int32_t )array[ 1 ] << " "
            << std::setw( 3 ) << ( int32_t )array[ 2 ] << " "
            << std::setw( 3 ) << ( int32_t )array[ 3 ] << " "
            << std::setw( 3 ) << ( int32_t )array[ 4 ] << " "
            << std::setw( 3 ) << ( int32_t )array[ 5 ] << " "
            << " ... "
            << std::setw( 3 ) << ( int32_t )array[ count - 1 ] << " "
            << "}" << std::endl;

  delete [] array;

}


bool Reader::getBinary()
{

  return true;

}


//
// class Service
//

Service::Service( in_port_t port )
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
// callback
//
void Callback( char* msg )
{

  std::cout << "msg=" << msg << std::endl;

}


//
// main procedure
//
int main() 
{

  Service* service = new Service( 8001 );

  service->run();

  delete service;

  return EXIT_SUCCESS;

}
