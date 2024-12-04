#include <gkg-communication-rpc/RpcCommand.h>


gkg::RpcCommand::RpcCommand( int32_t id )
                : _id( id )
{
}


gkg::RpcCommand::~RpcCommand()
{
}


void gkg::RpcCommand::send( gkg::SocketStream& server )
{

  if ( server.good() )
  {
 
    server.write( (const char*)&_id, sizeof( int32_t ) );
    
  }

}
