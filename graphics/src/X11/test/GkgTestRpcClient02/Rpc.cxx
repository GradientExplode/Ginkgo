#include <gkg-communication-rpc/RpcReader.h>
#include <gkg-communication-socket/SocketService.h>
#include <gkg-communication-socket/SocketHeader.h>
#include <gkg-communication-rpc/RpcProlog.h>
#include <gkg-communication-rpc/RpcEpilog.h>
#include <gkg-communication-socket/SocketStream.h>
#include "Rpc.h"


Writer::Writer( in_port_t port )
       : gkg::RpcWriter( "localhost", port, getFatal(), getBinary() )
{

  setProlog( "GkgTestRpcServer02" );
  setEpilog( "GkgTestRpcServer02" );

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


bool Writer::getFatal()
{

  return false;

}


bool Writer::getBinary()
{

  return true;

}
