#include <gkg-communication-rpc/RpcReader.h>
#include <gkg-communication-socket/SocketService.h>
#include <gkg-communication-socket/SocketHeader.h>
#include <gkg-communication-socket/SocketStream.h>
#include <gkg-communication-rpc/RpcWriter.h>
#include <gkg-graphics-X11-core/Session.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>


class Reader : public gkg::RpcReader
{

  public:

    Reader( int32_t port, gkg::SocketService* service );
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

    Service( int32_t port );
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

Reader::Reader( int32_t port, gkg::SocketService* service )
       : gkg::RpcReader( port, getBinary(), false ),
         _service(service)
{

  setProlog( "GkgTestRpcServer04" );
  setEpilog( "GkgTestRpcServer04" );
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
// callback
//
void Callback( char* msg )
{

  if ( strcmp( msg, "exit" ) == 0 )
  {

    gkg::Session::getInstance().quit();

  }
  else
  {

    std::cout << "msg=" << msg << std::endl;

  } 

}


//
// main procedure
//
int main( int argc, char* argv[] ) 
{

  Service* service = new Service( 8001 );

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestRpcServer04", argc, argv );

  int result = session.run();

  delete service;

  return result;

}
