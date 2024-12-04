#ifndef _gkg_graphics_X11_plugin_functors_Viewer2dRpcServer_RpcUpdateCallback_h_
#define _gkg_graphics_X11_plugin_functors_Viewer2dRpcServer_RpcUpdateCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>


namespace gkg
{


class RpcUpdateCallback : public RpcCallback
{

  public:
  
    RpcUpdateCallback( int32_t id, void* parameter );
    
    void receive( SocketStream& stream );

};


}


#endif
