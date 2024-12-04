// RPC inter-process communication channel : sender
// Open a terminal, and launch GkgTestRpcServer01
// Open a new terminal, and launch GkgTestRpcClient01

#include <gkg-communication-rpc/RpcReader.h>
#include <gkg-communication-socket/SocketService.h>
#include <gkg-communication-socket/SocketHeader.h>
#include <gkg-communication-rpc/RpcProlog.h>
#include <gkg-communication-rpc/RpcEpilog.h>
#include <gkg-communication-socket/SocketStream.h>
#include <gkg-communication-rpc/RpcWriter.h>

#include <iostream>
#include <cstdlib>


class Writer : public gkg::RpcWriter
{

  public:

    Writer( in_port_t port );

    void send( char* text );
    void flush();

  protected:

    bool getFatal();
    bool getBinary();

};


Writer::Writer( in_port_t port )
       : gkg::RpcWriter( "localhost", port, getFatal(), getBinary() )
{

  setProlog( "GkgTestRpcServer01" );
  setEpilog( "GkgTestRpcServer01" );

}


void Writer::flush()
{

  getServer().flush();

}


void Writer::send( char* text )
{

  gkg::SocketHeader header( this, 0 );
  gkg::RpcProlog prolog( _prolog );
  gkg::RpcEpilog epilog( _epilog );
  getServer() << prolog << header << text << epilog;

}


bool Writer::getFatal()
{

  return false;

}


bool Writer::getBinary()
{

  return true;

}


int main()
{

  Writer* writer = new Writer( 8001 );

  writer->send( ( char* )"hello, this is GkgTestRpcClient01!" );
  writer->flush();

  writer->send( ( char* )"exit" );
  writer->flush();

  delete writer;

  return EXIT_SUCCESS;

}
