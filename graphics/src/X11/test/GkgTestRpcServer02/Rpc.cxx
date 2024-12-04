#include <gkg-communication-socket/SocketHeader.h>
#include <gkg-communication-socket/SocketStream.h>
#include <gkg-communication-rpc/RpcReader.h>
#include <iostream>
#include "Rpc.h"


//
// class Reader
//

Reader::Reader( int32_t port, gkg::SocketService* service )
       : gkg::RpcReader( port, getBinary() ),
         _service( service )
{

  setProlog( "GkgTestRpcServer02" );
  setEpilog( "GkgTestRpcServer02" );
  addFunction( Reader::STRING, &Reader::receiveString );
  addFunction( Reader::INTEGER, &Reader::receiveInt );

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


bool Reader::getBinary()
{

  return false;

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


