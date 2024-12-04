#ifndef _gkg_communication_rpc_RpcCommand_h_
#define _gkg_communication_rpc_RpcCommand_h_


#include <gkg-communication-socket/SocketStream.h>


namespace gkg
{


class OStream;
class IStream;


class RpcCommand
{

  public:
  
    RpcCommand( int32_t id );
    virtual ~RpcCommand();
        
    virtual void send( SocketStream& server );

  private:
  
    int32_t _id;

};


}


#endif

