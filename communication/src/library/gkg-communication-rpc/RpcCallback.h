#ifndef _gkg_communication_rpc_RpcCallback_h_
#define _gkg_communication_rpc_RpcCallback_h_


#include <gkg-communication-socket/SocketStream.h>


namespace gkg
{


class RpcCallback
{

  public:
    
    RpcCallback( int32_t id, void* parameter );
    virtual ~RpcCallback();

    int32_t getId();
    
    virtual void receive( SocketStream& stream ) = 0;

  protected:
  
     void* _parameter;
     
 private:
  
    int32_t _id;

};


}


#endif
