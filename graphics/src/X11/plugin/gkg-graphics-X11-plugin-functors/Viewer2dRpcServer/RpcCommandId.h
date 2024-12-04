#ifndef _gkg_graphics_X11_plugin_functors_Viewer2dRpcServer_RpcCommandId_h_
#define _gkg_graphics_X11_plugin_functors_Viewer2dRpcServer_RpcCommandId_h_


namespace gkg
{


enum RpcCommandId
{

  RESET_VIEWER,
  UPDATE_VIEWER,
  EXIT_VIEWER,
  TRANSFER_VOLUME_INT16,
  TRANSFER_VOLUME_FLOAT,
  TRANSFER_VOLUME_RGB,
  COMMAND_COUNT

};


}


#endif
