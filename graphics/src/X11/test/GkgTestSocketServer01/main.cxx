#include <gkg-communication-socket/SocketReader.h>
#include <gkg-communication-socket/SocketService.h>
#include <gkg-communication-socket/SocketHeader.h>
#include <gkg-communication-socket/SocketStream.h>
#include <gkg-graphics-X11-core/Session.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>


class Reader : public gkg::SocketReader
{

  public:

    Reader( int32_t fd, gkg::SocketService* service );
    ~Reader();

  protected:

    enum
    {

      DIMENSION,
      RESOLUTION,
      DATA,
      FRAMECOUNT

    };

    static void receiveDimension( gkg::SocketReader* reader,
                                  gkg::SocketHeader& header,
                                  gkg::SocketStream& stream );
    static void receiveResolution( gkg::SocketReader* reader,
                                   gkg::SocketHeader& header,
                                   gkg::SocketStream& stream );
    static void receiveData( gkg::SocketReader* reader,
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

Reader::Reader( int32_t fd, gkg::SocketService* service )
       : gkg::SocketReader( fd, getBinary(), false ),
         _service(service)
{

  addFunction( Reader::DIMENSION, &Reader::receiveDimension );
  addFunction( Reader::RESOLUTION, &Reader::receiveResolution );
  addFunction( Reader::DATA, &Reader::receiveData );

}


Reader::~Reader()
{
}


void Reader::connectionClosed( int32_t )
{

  _service->quitRunning();

}


void Reader::receiveDimension( gkg::SocketReader*,
                               gkg::SocketHeader&,
                               gkg::SocketStream& stream )
{

  int32_t elementSize;
  int64_t i, elementCount;

  stream.read( (char*)&elementSize, sizeof( int32_t ) );
  stream.read( (char*)&elementCount, sizeof( int64_t ) );

  int32_t* buffer = new int32_t[ elementCount ];
  stream.read( (char*)buffer, elementSize * elementCount );

  for ( i = 0; i < elementCount - 1; i++ )
  {

    std::cout << buffer[ i ] << "x";

  }

  std::cout << buffer[ i ] << std::endl;

}


void Reader::receiveResolution( gkg::SocketReader*,
                                gkg::SocketHeader&,
                                gkg::SocketStream& stream )
{

  int32_t elementSize;
  int64_t i, elementCount;

  stream.read( (char*)&elementSize, sizeof( int32_t ) );
  stream.read( (char*)&elementCount, sizeof( int64_t ) );

  double* buffer = new double[ elementCount ];
  stream.read( (char*)buffer, elementSize * elementCount );

  for ( i = 0; i < elementCount - 1; i++ )
  {

    std::cout << buffer[ i ] << "x";

  }

  std::cout << buffer[ i ] << std::endl;

}


void Reader::receiveData( gkg::SocketReader*,
                         gkg::SocketHeader&,
                         gkg::SocketStream& stream )
{

  int32_t elementSize;
  int64_t i, elementCount;

  stream.read( (char*)&elementSize, sizeof( int32_t ) );
  stream.read( (char*)&elementCount, sizeof( int64_t ) );

  float* buffer = new float[ elementCount ];
  stream.readPackets( (char*)buffer, elementSize * elementCount );

  for ( i = 0; i < elementCount; i++ )
  {

    std::cout << buffer[ i ] << " ";

  }

  std::cout << std::endl;

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
// main procedure
//
int main( int argc, char* argv[] ) 
{

  Service* service = new Service( 8001 );

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestSocketServer01", argc, argv );

  int result = session.run();

  delete service;

  return result;

}
