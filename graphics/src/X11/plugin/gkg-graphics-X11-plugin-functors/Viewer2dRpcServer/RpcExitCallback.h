#ifndef _gkg_graphics_X11_plugin_functors_Viewer2dRpcServer_RpcExitCallback_h_
#define _gkg_graphics_X11_plugin_functors_Viewer2dRpcServer_RpcExitCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>


namespace gkg
{


class RpcExitCallback : public RpcCallback
{

  public:
  
    RpcExitCallback( int32_t id, void* parameter );
    
    void receive( SocketStream& stream );

};


}


#endif
