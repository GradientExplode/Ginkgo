#include <gkg-graphics-X11-plugin-functors/Viewer2dRpcServer/RpcUpdateCallback.h>
#include <gkg-graphics-X11-plugin-functors/Viewer2dRpcServer/Viewer2d.h>


gkg::RpcUpdateCallback::RpcUpdateCallback(  int32_t id, void* parameter )
                       : gkg::RpcCallback( id, parameter )
{
}


void gkg::RpcUpdateCallback::receive( SocketStream& )
{

  gkg::Viewer2d* viewer = reinterpret_cast< gkg::Viewer2d* >( _parameter );
  
  if ( viewer )
  {
  
    viewer->update();
  
  }

}
