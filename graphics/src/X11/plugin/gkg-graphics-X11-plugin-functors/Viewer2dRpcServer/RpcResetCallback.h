#ifndef _gkg_graphics_X11_plugin_functors_Viewer2dRpcServer_RpcResetCallback_h_
#define _gkg_graphics_X11_plugin_functors_Viewer2dRpcServer_RpcResetCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>


namespace gkg
{


class RpcResetCallback : public RpcCallback
{

  public:
  
    RpcResetCallback( int32_t id, void* parameter );
    
    void receive( SocketStream& stream );

};


}


#endif
