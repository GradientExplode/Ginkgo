#include <gkg-graphics-X11-plugin-functors/Viewer2dRpcServer/RpcExitCallback.h>
#include <gkg-graphics-X11-core/Session.h>


gkg::RpcExitCallback::RpcExitCallback(  int32_t id, void* parameter )
                     : gkg::RpcCallback( id, parameter )
{
}


void gkg::RpcExitCallback::receive( SocketStream& )
{

  gkg::Session::getInstance().quit();

}
