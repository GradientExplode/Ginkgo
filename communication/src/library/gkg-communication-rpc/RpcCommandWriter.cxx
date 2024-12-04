#include <gkg-communication-rpc/RpcCommandWriter.h>
#include <gkg-communication-socket/SocketHeader.h>
#include <gkg-communication-rpc/RpcProlog.h>
#include <gkg-communication-rpc/RpcEpilog.h>


gkg::RpcCommandWriter::RpcCommandWriter( const std::string& host,
                                         in_port_t port,
                                         bool noDelay )
	              : gkg::RpcWriter( host, 
                                        port, 
                                        getFatal(), 
                                        getBinary(),
                                        noDelay )
{

  setProlog( "RpcCommandWriter" );
  setEpilog( "RpcCommandWriter" );

}


void gkg::RpcCommandWriter::send( gkg::RpcCommand& command )
{

  gkg::SocketHeader header( this, SEND );
  gkg::RpcProlog prolog( _prolog );
  gkg::RpcEpilog epilog( _epilog );
  
  getServer() << prolog << header;
  
  command.send( getServer() );
  
  getServer() << epilog;

}


void gkg::RpcCommandWriter::flush()
{

  getServer().flush();

}


bool gkg::RpcCommandWriter::getFatal()
{

  return true;

}


bool gkg::RpcCommandWriter::getBinary()
{

  return true;
  
}
