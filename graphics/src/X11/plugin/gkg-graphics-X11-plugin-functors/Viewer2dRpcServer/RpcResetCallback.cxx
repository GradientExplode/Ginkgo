#include <gkg-graphics-X11-plugin-functors/Viewer2dRpcServer/RpcResetCallback.h>
#include <gkg-graphics-X11-plugin-functors/Viewer2dRpcServer/Viewer2d.h>


gkg::RpcResetCallback::RpcResetCallback(  int32_t id, void* parameter )
                      : gkg::RpcCallback( id, parameter )
{
}


void gkg::RpcResetCallback::receive( SocketStream& )
{

  gkg::Viewer2d* viewer = reinterpret_cast< gkg::Viewer2d* >( _parameter );
  
  if ( viewer )
  {
  
    viewer->reset( true );
  
  }

}
